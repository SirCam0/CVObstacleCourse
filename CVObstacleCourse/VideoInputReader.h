// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Actor for camera functionality - public

#pragma once

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include "PlayerChararcterClass.h"
#include "CVObstacleCourseGameModeBase.h"
#include "CoreMinimal.h"
#include <iostream>
#include "CVOCFunctions/HaarCascadeFunctions.h"
#include "MediaTexture.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "GameFramework/Actor.h"
#include "VideoInputReader.generated.h"

using namespace cv;
using namespace std;

UCLASS()
class CVOBSTACLECOURSE_API AVideoInputReader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVideoInputReader(const FObjectInitializer& ObjectInitializer);
    ~AVideoInputReader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void CVProcessing(Mat InputFrame);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
        USceneComponent* SceneRoot;
    
    // The rate at which the color data array and video texture is updated (in frames per second)
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OpenCV, meta = (ClampMin = 0, UIMin = 0))
        float RefreshRate;
    // The refresh timer
    UPROPERTY(BlueprintReadWrite, Category = OpenCV)
        float RefreshTimer;

    // is the camera stream on
    UPROPERTY(BlueprintReadWrite, Category = Input)
        bool isStreamOpen;

    // Blueprint Event called every time the video frame is updated
    UFUNCTION(BlueprintImplementableEvent, Category = OpenCV)
        void OnNextVideoFrame();
    
    // reads the current video frame
    UFUNCTION(BlueprintCallable, Category = Data)
        bool ReadFrame();

    UFUNCTION(BlueprintCallable)
        void UpdateInputMode();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Player)
        APlayerChararcterClass* Player;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Player)
        ACVObstacleCourseGameModeBase* CVGamemode;

    UPROPERTY()
    FTimerHandle StartDelayTimer;
    float StartDelayTime = 0.1f;

    UFUNCTION()
    void AfterStartDelay();
    
    //OpenCV
    VideoCapture camera;
    Size cvSize;
    Mat frame;
    Mat frameHSV, mask;

    //Texture setup
    UPROPERTY(BlueprintReadWrite, Category = Camera)
        FVector2D VideoSize;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
        UMaterialInstanceDynamic* CameraMatPost;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
        UMaterialInstanceDynamic* CameraMatRaw;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
        UMediaPlayer* CameraMediaPlayer;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
        UMediaTexture* CameraMediaTexture;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
        UTextureRenderTarget2D* CameraRenderTarget;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
    UTexture2D* CameraTexture2D;
    
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Data)
        TArray<FColor> ColourData;


    UFUNCTION(BlueprintCallable)
    void CloseCameraStream();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void OpenCameraStream();
    

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera, meta = (ClampMin = 0, UIMin = 0))
        int CenterInSet = 5;

};
