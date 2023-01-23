// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Computer Vision Obstacle Course Gamemode - Public

#pragma once

#include "CoreMinimal.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "GameFramework/GameModeBase.h"
#include "CVObstacleCourseFunctions.h"
#include "CVObstacleCourseSaveGame.h"
#include "CVOCGameModeInterface.h"
#include "CVObstacleCourseGameModeBase.generated.h"

using namespace cv;

class AVideoInputReader;
/**
 * 
 */
UCLASS()
class CVOBSTACLECOURSE_API ACVObstacleCourseGameModeBase : public AGameModeBase, public ICVOCGameModeInterface
{
	GENERATED_BODY()

public:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    ACVObstacleCourseGameModeBase();

    ACVObstacleCourseGameModeBase* GetGameModeREF_Implementation() override;

    //Open CV colour detection values
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int hmin = 30;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int smin = 40;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int vmin = 191;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int hmax = 72;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int smax = 132;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int vmax = 219;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FName> UnlockedLevels;

    // Set the default input mode
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TEnumAsByte<EInputMode> GameInputMode = ComputerVision;

    UFUNCTION(BlueprintCallable)
        FVector GetMaxAsRGB();
    UFUNCTION(BlueprintCallable)
        FVector GetMinAsRGB();
    UFUNCTION(BlueprintCallable)
        void SaveColourSettings();
    UFUNCTION(BlueprintCallable)
        void LoadColourSettings();
    UFUNCTION(BlueprintCallable)
        void UnlockLevel(FName LevelName);
    UFUNCTION(BlueprintCallable)
        void LoadUnlockedLevels();
    UFUNCTION(BlueprintCallable)
        void SetInputMode(TEnumAsByte<EInputMode> NewInputMode);
    UFUNCTION(BlueprintCallable)
        void LoadInputMode();
    UFUNCTION(BlueprintCallable)
        void SetCameraID(int32 ID);
    UFUNCTION(BlueprintCallable)
        void LoadCameraID();

    // Colour calibration window stuff
    UFUNCTION(BlueprintCallable)
        void OpenColourCalibrationMenu();
    UFUNCTION(BlueprintCallable)
        void CloseColourCalibrationMenu();
    UFUNCTION()
        void UpdateMask();
    // not a UFUNCTION as it is a data type that unreal does not recognize
        VideoCapture Camera;
    UPROPERTY(BlueprintReadWrite)
        int32 CameraID = 0;
    UPROPERTY()
        float MaskUpdateRate = 0.1f;
    UPROPERTY()
        FTimerHandle MaskUpdateTimer;
    UPROPERTY()
        AVideoInputReader* VideoInputReader;
    UPROPERTY()
        bool ColourCalibrationWindowOpen = false;

    // default values for player movement so that they can be easily accessed in the ice floor asset
    UPROPERTY(BlueprintReadWrite)
        float GroundFriction = 8;
    UPROPERTY(BlueprintReadWrite)
        float BrakingDeceleration = 2048;
    UPROPERTY(BlueprintReadWrite)
        float MaxAcceleration = 10000;

    // sound volume settings
    UPROPERTY(BlueprintReadWrite)
        int MasterVol = 10;
    UPROPERTY(BlueprintReadWrite)
        int MusicVol = 10;
    UPROPERTY(BlueprintReadWrite)
        int SFXVol = 10;
    UPROPERTY(BlueprintReadWrite)
        int UIVol = 10;

    // blueprint implementable to actually set the volumes of sound classes in engine
    UFUNCTION(BlueprintImplementableEvent)
        void SetNewMasterVolumeBP(int Vol);
    UFUNCTION(BlueprintImplementableEvent)
        void SetNewMusicVolumeBP(int Vol);
    UFUNCTION(BlueprintImplementableEvent)
        void SetNewSFXVolumeBP(int Vol);
    UFUNCTION(BlueprintImplementableEvent)
        void SetNewUIVolumeBP(int Vol);
    // Functions to pass through new volume settings to the blueprint functions and add them to the save game
    UFUNCTION(BlueprintCallable)
        void SetMasterVolume(int Vol);
    UFUNCTION(BlueprintCallable)
        void SetMusicVolume(int Vol);
    UFUNCTION(BlueprintCallable)
        void SetSFXVolume(int Vol);
    UFUNCTION(BlueprintCallable)
        void SetUIVolume(int Vol);
    UFUNCTION(BlueprintCallable)
        void LoadVolumeSettings();

    // video mode values/functions
    UPROPERTY(BlueprintReadWrite)
    bool bIsWindowedMode = false;
    UFUNCTION(BlueprintCallable)
        void SetVideoMode(bool Windowed);
    UFUNCTION(BlueprintCallable)
        void LoadVideoMode();

};
