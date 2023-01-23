// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Save Game Public

// a save game object can be saved as a non-volatile object that can stay the same between launches of the game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CVOCSaveGameInterface.h"
#include "StructsAndEnums/Enums/EInputMode.h"
#include "CVObstacleCourseSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CVOBSTACLECOURSE_API UCVObstacleCourseSaveGame : public USaveGame, public ICVOCSaveGameInterface
{
	GENERATED_BODY()

public:

    UCVObstacleCourseSaveGame* GetSaveGameREF_Implementation() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int hmin = 0;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int smin = 0;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int vmin = 0;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int hmax = 0;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int smax = 0;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int vmax = 0;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FName> UnlockedLevels;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TEnumAsByte<EInputMode> InputMode = ComputerVision;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 CameraID;
    UPROPERTY(BlueprintReadWrite)
    int MasterVolume = 6;
    UPROPERTY(BlueprintReadWrite)
    int MusicVolume = 6;
    UPROPERTY(BlueprintReadWrite)
    int SFXVolume = 6;
    UPROPERTY(BlueprintReadWrite)
    int UIVolume = 6;
    UPROPERTY(BlueprintReadWrite)
    bool WindowedMode = false;
};
