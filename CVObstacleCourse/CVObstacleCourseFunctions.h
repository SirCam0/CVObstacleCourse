// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// A Function library that can be called from anywhere within the unreal project C++ or blueprint - Public

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CVObstacleCourseFunctions.generated.h"

/**
 * 
 */
UCLASS()
class CVOBSTACLECOURSE_API UCVObstacleCourseFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static FVector HSVtoRGB(FVector HSV);
	UFUNCTION(BlueprintCallable)
	static FVector RGBtoHSV(FVector RGB);
};
