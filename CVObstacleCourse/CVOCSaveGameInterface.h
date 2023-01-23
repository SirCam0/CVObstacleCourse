// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// in interface for the save game to allow function calls without casting
// no private class is required as we are not implementing the function in this class

#pragma once

#include "CVOCSaveGameInterface.generated.h"

class UCVObstacleCourseSaveGame;
/**
 * 
 */
UINTERFACE(Blueprintable)
class UCVOCSaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

class ICVOCSaveGameInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = SaveGameInterface)
		UCVObstacleCourseSaveGame* GetSaveGameREF();
};