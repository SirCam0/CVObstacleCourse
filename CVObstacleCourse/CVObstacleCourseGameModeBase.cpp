// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Computer Vision Obstacle Course Gamemode - Private

/** Improvements to be made:
 *	add a function in the constructor to initialize the save game so that there doesnt have to ba an initialize clause in each save function
 *	add a load save function as the save is loaded in every function that requires the save game, this would save a few lines of code
 */

#include "CVObstacleCourseGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "TimerManager.h"
#include "VideoInputReader.h"
#include "GameFramework/GameUserSettings.h"

void ACVObstacleCourseGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// load all values from the save game
	LoadColourSettings();
	LoadUnlockedLevels();
	LoadInputMode();
	LoadCameraID();
	LoadVolumeSettings();
	LoadVideoMode();
}

ACVObstacleCourseGameModeBase::ACVObstacleCourseGameModeBase()
{
	// make sure that level one is unlocked at the start of the game
	UnlockLevel("LEVEL1");
}

// interface function override to return a pointer to this object to avoid casting
ACVObstacleCourseGameModeBase* ACVObstacleCourseGameModeBase::GetGameModeREF_Implementation()
{
	return this;
}

// function to get the HSV Max value as a RGB Value
FVector ACVObstacleCourseGameModeBase::GetMaxAsRGB()
{
	const FVector RGBMax = UCVObstacleCourseFunctions::HSVtoRGB(FVector(hmax, smax, vmax));
	return RGBMax;
}

// function to get the HSV Min value as a RGB Value
FVector ACVObstacleCourseGameModeBase::GetMinAsRGB()
{
	const FVector RGBMin = UCVObstacleCourseFunctions::HSVtoRGB(FVector(hmin, smin, vmin));
	return RGBMin;
}

// function to add the current colour input settings to the save game
void ACVObstacleCourseGameModeBase::SaveColourSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->hmax = this->hmax;
		Save->smax = this->smax;
		Save->vmax = this->vmax;
		Save->hmin = this->hmin;
		Save->smin = this->smin;
		Save->vmin = this->vmin;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else{ // if the save game has not been initialized
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->hmax = this->hmax;
		Save->smax = this->smax;
		Save->vmax = this->vmax;
		Save->hmin = this->hmin;
		Save->smin = this->smin;
		Save->vmin = this->vmin;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
}

// function to get the current colour settings from the save game
void ACVObstacleCourseGameModeBase::LoadColourSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		this->hmax = Save->hmax;
		this->smax = Save->smax;
		this->vmax = Save->vmax;
		this->hmin = Save->hmin;
		this->smin = Save->smin;
		this->vmin = Save->vmin;
	}
}

// Function to unlock a level
void ACVObstacleCourseGameModeBase::UnlockLevel(FName LevelName)
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		bool used = false;
		// check if this level has been unlocked already
		for (FName level : Save->UnlockedLevels) {
			if (level == LevelName) {
				used = true;
			}
		}
		if (!used) {
			Save->UnlockedLevels.Add(LevelName);
		}
		this->UnlockedLevels = Save->UnlockedLevels;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else { // if the save game has not been initialized
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->UnlockedLevels = this->UnlockedLevels;
		
		// we do nt need to check if the level has been unlocked already as this is a fresh save
		Save->UnlockedLevels.Add(LevelName);
		this->UnlockedLevels = Save->UnlockedLevels;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
}

// Load unlocked levels from the save game
void ACVObstacleCourseGameModeBase::LoadUnlockedLevels()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		this->UnlockedLevels = Save->UnlockedLevels;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
}

// Set the current input mode and add it to the save game
void ACVObstacleCourseGameModeBase::SetInputMode(TEnumAsByte<EInputMode> NewInputMode)
{
	VideoInputReader = Cast<AVideoInputReader>(UGameplayStatics::GetActorOfClass(this, AVideoInputReader::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->InputMode = NewInputMode;
		this->GameInputMode = Save->InputMode;
		VideoInputReader->UpdateInputMode();
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->InputMode = NewInputMode;
		this->GameInputMode = Save->InputMode;
		VideoInputReader->UpdateInputMode();
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
}

// load the current input mode from the save game
void ACVObstacleCourseGameModeBase::LoadInputMode()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		this->GameInputMode = Save->InputMode;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
}

// set the camera ID for computer vision and add it to the save game
void ACVObstacleCourseGameModeBase::SetCameraID(int32 ID)
{
	VideoInputReader = Cast<AVideoInputReader>(UGameplayStatics::GetActorOfClass(this, AVideoInputReader::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->CameraID = ID;
		this->CameraID = ID;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->CameraID = ID;
		this->CameraID = ID;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
}

// load the camera ID from the save game
void ACVObstacleCourseGameModeBase::LoadCameraID()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		this->CameraID = Save->CameraID;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
}

// function to open an openCV window with trackbars for each HSV Setting
void ACVObstacleCourseGameModeBase::OpenColourCalibrationMenu()
{
	VideoInputReader = Cast<AVideoInputReader>(UGameplayStatics::GetActorOfClass(this, AVideoInputReader::StaticClass()));
	if (VideoInputReader->IsValidLowLevelFast()) {
		VideoInputReader->isStreamOpen = false;
		VideoInputReader->CloseCameraStream();
		Camera = VideoCapture(CameraID);
		namedWindow("ColourSettings", (640, 200));
		createTrackbar("Hue min", "ColourSettings", &hmin, 179);
		createTrackbar("Saturation min", "ColourSettings", &smin, 255);
		createTrackbar("Value min", "ColourSettings", &vmin, 255);
		createTrackbar("Hue max", "ColourSettings", &hmax, 179);
		createTrackbar("Saturation max", "ColourSettings", &smax, 255);
		createTrackbar("Value max", "ColourSettings", &vmax, 255);
		GetWorld()->GetTimerManager().SetTimer(MaskUpdateTimer, this, &ACVObstacleCourseGameModeBase::UpdateMask, MaskUpdateRate, true, 0);
		ColourCalibrationWindowOpen = true;
	}
}

// Open/update an opencv window showing the HSV mask for colour detection
void ACVObstacleCourseGameModeBase::UpdateMask() {
	Mat frame;
	
	Camera.read(frame);
	
	Mat imgHSV, mask;

	cvtColor(frame, imgHSV, COLOR_BGR2HSV);

	Scalar lower(hmin, smin, vmin); // hue, saturation, value
	Scalar upper(hmax, smax, vmax);

	inRange(imgHSV, lower, upper, mask);
	
	putText(mask, "Click save in the settings menu to", Point(10, 20), FONT_HERSHEY_COMPLEX, 1, Scalar(0,255,255), 1);
	putText(mask, "close this window and save changes", Point(10, 45), FONT_HERSHEY_COMPLEX, 1, Scalar(0,255,255), 1);
	imshow("FrameWindow", mask);
	
}

// Set the master volume value and add it to the save game
void ACVObstacleCourseGameModeBase::SetMasterVolume(int Vol)
{
	VideoInputReader = Cast<AVideoInputReader>(UGameplayStatics::GetActorOfClass(this, AVideoInputReader::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->MasterVolume = Vol;
		this->MasterVol = Vol;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->MasterVolume = Vol;
		this->MasterVol = Vol;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	// set the actual volume settings in BP as i have set up all of my sound classes in BP
	SetNewMasterVolumeBP(Vol);
}

// Set the music volume value and add it to the save game
void ACVObstacleCourseGameModeBase::SetMusicVolume(int Vol)
{
	VideoInputReader = Cast<AVideoInputReader>(UGameplayStatics::GetActorOfClass(this, AVideoInputReader::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->MusicVolume = Vol;
		this->MusicVol = Vol;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->MusicVolume = Vol;
		this->MusicVol = Vol;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	// set the actual volume settings in BP as i have set up all of my sound classes in BP
	SetNewMusicVolumeBP(Vol);
}

// Set the SFX volume value and add it to the save game
void ACVObstacleCourseGameModeBase::SetSFXVolume(int Vol)
{
	VideoInputReader = Cast<AVideoInputReader>(UGameplayStatics::GetActorOfClass(this, AVideoInputReader::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->SFXVolume = Vol;
		this->SFXVol = Vol;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->SFXVolume = Vol;
		this->SFXVol = Vol;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	// set the actual volume settings in BP as i have set up all of my sound classes in BP
	SetNewSFXVolumeBP(Vol);
}

// Set the UI volume value and add it to the save game
void ACVObstacleCourseGameModeBase::SetUIVolume(int Vol)
{
	VideoInputReader = Cast<AVideoInputReader>(UGameplayStatics::GetActorOfClass(this, AVideoInputReader::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->UIVolume = Vol;
		this->UIVol = Vol;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->UIVolume = Vol;
		this->UIVol = Vol;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	// set the actual volume settings in BP as i have set up all of my sound classes in BP
	SetNewUIVolumeBP(Vol);
}

// load the volume settings from the save game and set them in BP
void ACVObstacleCourseGameModeBase::LoadVolumeSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		this->MasterVol = Save->MasterVolume;
		this->MusicVol = Save->MusicVolume;
		this->SFXVol = Save->SFXVolume;
		this->UIVol = Save->UIVolume;
	}
	// set the actual volume settings in BP as i have set up all of my sound classes in BP
	SetNewMasterVolumeBP(this->MasterVol);
	SetNewMusicVolumeBP(this->MusicVol);
	SetNewSFXVolumeBP(this->SFXVol);
	SetNewUIVolumeBP(this->UIVol);
}

// set the video mode to either windowed or fullscreen and add it to the save game
void ACVObstacleCourseGameModeBase::SetVideoMode(bool Windowed)
{
	VideoInputReader = Cast<AVideoInputReader>(UGameplayStatics::GetActorOfClass(this, AVideoInputReader::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->WindowedMode = Windowed;
		this->bIsWindowedMode = Windowed;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	else{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCVObstacleCourseSaveGame::StaticClass());
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		Save->WindowedMode = Windowed;
		this->bIsWindowedMode = Windowed;
		UGameplayStatics::SaveGameToSlot(Save, TEXT("Save"), 0);
	}
	if(bIsWindowedMode)
	{
		GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
		GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(1280,720));
	}
	else
	{
		GEngine->GetGameUserSettings()->SetScreenResolution(GEngine->GetGameUserSettings()->GetDesktopResolution());
		GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
	}
	GEngine->GetGameUserSettings()->ApplySettings(false);
}

// load the video mode from the save game and setup the screen
void ACVObstacleCourseGameModeBase::LoadVideoMode()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Save"), 0)) {
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(TEXT("save"), 0);
		UCVObstacleCourseSaveGame* Save = ICVOCSaveGameInterface::Execute_GetSaveGameREF(SaveGame);
		this->bIsWindowedMode = Save->WindowedMode;
	}
	if(bIsWindowedMode)
	{
		GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
		GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(1280,720));
	}
	else
	{
		GEngine->GetGameUserSettings()->SetScreenResolution(GEngine->GetGameUserSettings()->GetDesktopResolution());
		GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
	}
	GEngine->GetGameUserSettings()->ApplySettings(false);
}

// close the opencv windows for colour calibrations and set the updated HSV values in the save game
void ACVObstacleCourseGameModeBase::CloseColourCalibrationMenu()
{
	if(ColourCalibrationWindowOpen)
	{
		GetWorld()->GetTimerManager().ClearTimer(MaskUpdateTimer);
		if(GameInputMode != Keyboard)
		{
			VideoInputReader->isStreamOpen = true;
		}
		VideoInputReader->OpenCameraStream();
		ColourCalibrationWindowOpen = false;
		Camera.release();
		destroyAllWindows();
		SaveColourSettings();
	}
}
