// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Save game private


#include "CVObstacleCourseSaveGame.h"

// interface function override to return a pointer to this object to avoid casting
UCVObstacleCourseSaveGame* UCVObstacleCourseSaveGame::GetSaveGameREF_Implementation() {
	return this;
}