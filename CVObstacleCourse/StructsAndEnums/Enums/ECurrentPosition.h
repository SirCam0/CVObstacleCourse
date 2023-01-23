// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// an enum to represent the players current position in the level - no longer used

#pragma once

UENUM(BlueprintType)
enum ECurrentPosition
{
	Left UMETA(DisplayName = "Left"),
	Center UMETA(DisplayName = "Center"),
	Right UMETA(DisplayName = "Right"),
};
