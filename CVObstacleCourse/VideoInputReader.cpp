// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Actor for camera functionality - private


#include "VideoInputReader.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

#include "IMediaCaptureSupport.h"
#include "MediaCaptureSupport.h"
#include "MediaPlayer.h"
#include "TimerManager.h"
#include "AVEncoder/Public/MediaPacket.h"
#include "Misc/MediaBlueprintFunctionLibrary.h"

//VideoCapture webcam(0);

// Sets default values
AVideoInputReader::AVideoInputReader(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set up a scene component for in world placement
	SceneRoot = CreateDefaultSubobject<USceneComponent>("RootComp");
	RootComponent = SceneRoot;

	// setup base variables
	isStreamOpen = false;
	RefreshRate = 10.f;
	VideoSize = FVector2D(256,256);
}

// when the actor is destroyed close all open cv windows
AVideoInputReader::~AVideoInputReader()
{
	destroyAllWindows();
}

// Called when the game starts or when spawned
void AVideoInputReader::BeginPlay()
{
	Super::BeginPlay();

	// get a reference for the game mode using an interface call
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode->Implements<UCVOCGameModeInterface>()) {
		CVGamemode = ICVOCGameModeInterface::Execute_GetGameModeREF(GameMode);
	}

	// get a reference for the player character using an interface call
	AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerActor->Implements<UCVOCPlayerInterface>()) {
		Player = ICVOCPlayerInterface::Execute_GetPlayerREF(PlayerActor);
	}

	// setup the pixel array
	ColourData.AddDefaulted(VideoSize.X * VideoSize.Y);

	// setup the frame
	frame = Mat(cvSize, CV_8UC4, ColourData.GetData());

	// setup the texture2D asset
	CameraTexture2D = UTexture2D::CreateTransient(VideoSize.X,VideoSize.Y,PF_B8G8R8A8);

	// setup the camera render target
	CameraRenderTarget->SizeX = VideoSize.X;
	CameraRenderTarget->SizeY = VideoSize.Y;

#if WITH_EDITORONLY_DATA
	CameraTexture2D->MipGenSettings = TMGS_NoMipmaps;
#endif
	CameraTexture2D->SRGB = CameraRenderTarget->SRGB;

	// do a function after a delay
	GetWorld()->GetTimerManager().SetTimer(StartDelayTimer, this, &AVideoInputReader::AfterStartDelay, StartDelayTime, false);
}

// setup the input mode
void AVideoInputReader::UpdateInputMode()
{
	if(CVGamemode->GameInputMode == ComputerVision)
	{
		OpenCameraStream();
		this->isStreamOpen = true;
	}
	else
	{
		this->isStreamOpen = false;
	}
}

void AVideoInputReader::AfterStartDelay()
{
	UpdateInputMode();
}

void AVideoInputReader::CloseCameraStream()
{
	CameraMediaPlayer->Close();
}

// Called every frame
void AVideoInputReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(isStreamOpen == true)
	{
		// call the read frame function on the correct refresh rate
		RefreshTimer += DeltaTime;
		if (RefreshTimer >= 1.f / RefreshRate) {
			RefreshTimer -= 1.f / RefreshRate;
			ReadFrame();
			OnNextVideoFrame();
		}
	}
}

void AVideoInputReader::CVProcessing(Mat InputFrame)
{
	// convert the frame to HSV 	
	cvtColor(InputFrame, frameHSV, COLOR_BGR2HSV);
	Scalar lower(CVGamemode->hmin, CVGamemode->smin, CVGamemode->vmin); // hue, saturation, value
	Scalar upper(CVGamemode->hmax, CVGamemode->smax, CVGamemode->vmax);

	// setup some variables
	bool Smiling = false;

	// i have used cv namespace here because rect has some collisions with UE
	cv::Rect SmileLoc;
	bool ContainsFace = false;
	// place the default face location in the center
	Point FaceLoc = Point(InputFrame.cols / 2.f, InputFrame.rows / 2.f);
	
	// my own function from the CVOCFunctionLibrary.dll that i created
	/*
	 * These functions are done in a dll because unreal engine cannot correctly garbage collect std::vectors which are needed
	 * UE also crashes when using the get contours cv function
	 */
	ScanForFace(InputFrame,1.1,10,1.1,13,Smiling,SmileLoc,ContainsFace,FaceLoc);

	// Setup the left and right boundaries
	int RightX = (InputFrame.cols / 3) + CenterInSet;
	int LeftX = ((InputFrame.cols / 3) * 2) - CenterInSet;

	// setup the location string
	string currentMovement = "No Face";
	if (ContainsFace) {
		if (FaceLoc.x <= RightX) {
			currentMovement = "right";
			if(!Player->MovingRight)
			{
				Player->MoveRight();
			}
		}
		else if (FaceLoc.x >= LeftX) {
			currentMovement = "left ";
			if(!Player->MovingLeft)
			{
				Player->MoveLeft();
			}
		}
		else {
			currentMovement = "center";
			Player->StopHorizontalMovement();
		}
	}

	if(Smiling){
		// draw a rectangle where the smile is
		rectangle(InputFrame, SmileLoc, cv::Scalar(100, 0, 255), 2);
		Player->JumpEvent();
	}

		/** a different method for colour detection
		//Reduce noise for colour detection
		Mat dilImg, finImg;
		Mat kernel = getStructuringElement(MORPH_RECT, Size(10, 10));
		dilate(mask, dilImg, kernel);
		Mat kernel2 = getStructuringElement(MORPH_RECT, Size(15, 15));
		erode(dilImg, dilImg, kernel2);

		// if there are more than 100 pixels that have a value different to 0 (that are not black)
		if (countNonZero(mask) >= 100) {
			circle(frame, Point(25, 25), 20, Scalar(0, 255, 0), FILLED);
			if (!Player->MoveForward) {
				Player->ToggleMoveForward();
			}
		}
		else {
			circle(frame, Point(25, 25), 20, Scalar(0, 0, 255), FILLED);
			if (Player->MoveForward) {
				Player->ToggleMoveForward();
			}
		}
		*/
	
	int NumberOfObjectsDetected;

	// flip the frame so it is more like a mirror
	flip(InputFrame, InputFrame, 180);

	// my own function from my CVOCFunctionLibrary.dll that detects a colour in range upper-lower and draws a rectangle around it
	/*
	 * These functions are done in a dll because unreal engine cannot correctly garbage collect std::vectors which are needed
	 * UE also crashes when using the get contours cv function
	 */
	RectColour(InputFrame, upper, lower, 300, NumberOfObjectsDetected);

	// if the colour is detected
	if (NumberOfObjectsDetected >= 1) {
		circle(frame, Point(25, 25), 20, Scalar(0, 255, 0), FILLED);
		if (!Player->MoveForward) {
			Player->ToggleMoveForward();
		}
	}
	else{
		circle(frame, Point(25, 25), 20, Scalar(0, 0, 255), FILLED);
		if (Player->MoveForward) {
			Player->ToggleMoveForward();
		}
	}

	// draw text on the output image
	putText(InputFrame, currentMovement, Point(20, InputFrame.rows - 20), FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 255), 2);
	putText(InputFrame, to_string(NumberOfObjectsDetected), Point(InputFrame.cols - 70, InputFrame.rows - 20), FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 0, 255), 2);

	// draw lines to show the seperate areas
	line(InputFrame, Point(LeftX, InputFrame.rows), Point(LeftX, 0), Scalar(0, 0, 0), 2);
	line(InputFrame, Point(RightX, InputFrame.rows), Point(RightX, 0), Scalar(0, 0, 0), 2);
}


bool AVideoInputReader::ReadFrame()
{
	// if the camera texture or render target is not setup correctly then dont attempt to read frame
	if(!CameraTexture2D || !CameraRenderTarget) return false;

	// setup the render target, this is basically the camera, it is set in Blueprint
	FRenderTarget* RenderTarget = CameraRenderTarget->GameThread_GetRenderTargetResource();
	
	// read the camera frame and store the pixel data in an array
	RenderTarget->ReadPixels(ColourData);
	
	// set the viewport size
	cvSize = Size(VideoSize.X, VideoSize.Y);
	
	frame = Mat(cvSize,CV_8UC4,ColourData.GetData());
	frame.convertTo(frame, -1, 1,-1);

	// do all of the open cv processing on the frame before rendering it onto the screen
	CVProcessing(frame);
		
	// Lock the texture in memory so that its address does not change so it can be modified
	void* TextureData = CameraTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	const int32 TextureDataSize = ColourData.Num() * 4;

	// set the texture data as the pixel data array
	FMemory::Memcpy(TextureData, ColourData.GetData(), TextureDataSize);
		
	// Unlock the texture
	CameraTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	
	CameraTexture2D->UpdateResource();

	return true;
}
