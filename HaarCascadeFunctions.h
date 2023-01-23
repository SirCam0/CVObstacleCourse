// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Function library for open cv image processing - public
#pragma once

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include <iostream>

#ifdef CVOBSTACLECOURSEFUNCTIONS_EXPORTS
#define CVOBSTACLECOURSEFUNCTIONS_API __declspec(dllexport)
#else
#define CVOBSTACLECOURSEFUNCTIONS_API __declspec(dllimport)
#endif

using namespace cv;
using namespace std;

extern "C" CVOBSTACLECOURSEFUNCTIONS_API void ScanForFace(Mat InputImage, float FaceScaleFactor, float FaceMinNeighbours, float SmileScaleFactor, float SmileMinNeighbours, bool& Smiling, cv::Rect & SmileLoc, bool& ContainsFace, Point & FaceLoc);

extern "C" CVOBSTACLECOURSEFUNCTIONS_API void RectColour(Mat InputImage, Scalar Upper, Scalar Lower, int FilterSize, int& NumberOfObjectsDetected);