// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Function library for open cv image processing - private

#include "pch.h"
#include <utility>
#include <limits.h>
#include "HaarCascadeFunctions.h"

CVOBSTACLECOURSEFUNCTIONS_API void ScanForFace(Mat InputImage, float FaceScaleFactor, float FaceMinNeighbours, float SmileScaleFactor, float SmileMinNeighbours, bool& Smiling, cv::Rect& SmileLoc, bool& ContainsFace, Point& FaceLoc)
{
	// i have used the cv namespace for Rect values as iot may have some collisions when used with UE without the namespace
	CascadeClassifier FaceCascade;
	CascadeClassifier SmileCascade;
	vector<cv::Rect> faces;
	vector<cv::Rect> smiles;

	FaceCascade.load("T:/Year3Uni/AdvancedGamesDevelopment/CVObstacleCourse/UnrealProject/ThirdParty/Resources/haarcascade_frontalface_default.xml");
	SmileCascade.load("T:/Year3Uni/AdvancedGamesDevelopment/CVObstacleCourse/UnrealProject/ThirdParty/Resources/haarcascade_smile.xml");

	// if the cascades are not loaded correctly
	if (FaceCascade.empty() || SmileCascade.empty()) {
		cout << "Cascade not loaded" << endl;
		return;
	}

	//Face Detection
	FaceCascade.detectMultiScale(InputImage, faces, FaceScaleFactor, FaceMinNeighbours);

	if (!faces.empty()) {

		ContainsFace = true;

		// calculate the center point of the face rect
		cv::Rect face = faces[0];
		int width = (face.tl().x) - (face.br().x);
		int height = (face.tl().y) - (face.br().y);
		int x = (face.tl().x) - (width / 2);
		int y = (face.tl().y) - (height / 2);
		FaceLoc = Point(x, y);


		//Smile Detection
		SmileCascade.detectMultiScale(InputImage, smiles, SmileScaleFactor, SmileMinNeighbours);


		if (!smiles.empty()) {
			vector<cv::Rect> SmilesFiltered;
			// filter to see if a smile is on a face or if it is a false detection
			for (cv::Rect smile : smiles) {
				bool valid = faces[0].contains(smile.tl()) && faces[0].contains(smile.br());
				if (valid) {
					SmilesFiltered.push_back(smile);
				}
			}
			if (!SmilesFiltered.empty()) {
				SmileLoc = SmilesFiltered[0];
				Smiling = true;
			}
		}

	}
}

CVOBSTACLECOURSEFUNCTIONS_API void RectColour(Mat InputImage, Scalar Upper, Scalar Lower, int FilterSize, int& NumberOfObjectsDetected)
{
	Mat dilImg = InputImage;
	vector<vector<Point>> contours;
	vector<Vec4i> heierachy;

	// convert to hsv
	cvtColor(dilImg, dilImg, COLOR_BGR2HSV);
	// convert to a mask
	inRange(dilImg, Lower, Upper, dilImg);

	// get the contours of the detected colour mask
	findContours(dilImg, contours, heierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(InputImage, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<cv::Rect> boundRects(contours.size());

	// Filter the contours to a certain size
	int count = 0;
	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);

		if (area > FilterSize) {
			float peram = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peram, true);
			//drawContours(OutputImage, conPoly, i, Scalar(255, 0, 255), 2);
			int vertsNum = conPoly[i].size();

			// Create a rectancle arrounbd the detected vertecies
			boundRects[i] = boundingRect(conPoly[i]);
			rectangle(InputImage, boundRects[i], Scalar(50, 0, 255), 2);
			count++;
		}
	}
	NumberOfObjectsDetected = count;
}
