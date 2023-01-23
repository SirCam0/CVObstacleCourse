// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// in interface for the gamemode to allow function calls without casting
// no private class is required as we are not implementing the function in this class

#pragma once

#include "CVOCGameModeInterface.generated.h"

class ACVObstacleCourseGameModeBase;
/**
 * 
 */
UINTERFACE(Blueprintable)
class UCVOCGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class ICVOCGameModeInterface{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GamemodeInterface)
	ACVObstacleCourseGameModeBase* GetGameModeREF();
};
