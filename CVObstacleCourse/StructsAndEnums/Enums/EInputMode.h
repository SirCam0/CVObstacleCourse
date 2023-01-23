// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// An enum to represent the players input mode, so that it can pe expanded instead of just a bool

#pragma once

UENUM(BlueprintType)
enum EInputMode
{
	Keyboard UMETA(DisplayName = "Keyboard"),
	ComputerVision UMETA(DisplayName = "ComputerVision"),
};
