// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// in interface for the player to allow function calls without casting
// no private class is required as we are not implementing the function in this class

#pragma once

#include "CVOCPlayerInterface.generated.h"

class APlayerChararcterClass;
/**
 * 
 */
UINTERFACE(Blueprintable)
class UCVOCPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class ICVOCPlayerInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = PlayerInterface)
	APlayerChararcterClass* GetPlayerREF();
};
