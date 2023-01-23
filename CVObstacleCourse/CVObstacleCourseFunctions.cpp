// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// A Function library that can be called from anywhere within the unreal project C++ or blueprint - Private

#include "CVObstacleCourseFunctions.h"


// a function to convert HSV values to RGB
FVector UCVObstacleCourseFunctions::HSVtoRGB(FVector HSV)
{
	//X = H
	//Y = S
	//Z = V
	double r = 0, g = 0, b = 0;
	if (HSV.Y == 0)
	{
		r = HSV.Z;
		g = HSV.Z;
		b = HSV.Z;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (HSV.X == 360)
			HSV.X = 0;
		else
			HSV.X = HSV.X / 60;

		i = (int)trunc(HSV.X);
		f = HSV.X - i;

		p = HSV.Z * (1.0 - HSV.Y);
		q = HSV.Z * (1.0 - (HSV.Y * f));
		t = HSV.Z * (1.0 - (HSV.Y * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = HSV.Z;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = HSV.Z;
			b = p;
			break;

		case 2:
			r = p;
			g = HSV.Z;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = HSV.Z;
			break;

		case 4:
			r = t;
			g = p;
			b = HSV.Z;
			break;

		default:
			r = HSV.Z;
			g = p;
			b = q;
			break;
		}

	}
	return FVector((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255));
}

// a function to convert RGB to HSV
FVector UCVObstacleCourseFunctions::RGBtoHSV(FVector RGB)
{
	//X = H/R
	//Y = S/G
	//Z = V/B
	FVector HSV;
	double min, max, delta;

	min = RGB.X < RGB.Y ? RGB.X : RGB.Y;
	min = min < RGB.Z ? min : RGB.Z;

	max = RGB.X > RGB.Y ? RGB.X : RGB.Y;
	max = max > RGB.Z ? max : RGB.Z;

	HSV.Z = max;
	delta = max - min;
	if (delta < 0.00001)
	{
		HSV.Y = 0;
		HSV.X = 0;
		return HSV;
	}
	if (max > 0.0) {
		HSV.Y = (delta / max);
	}
	else {
		HSV.Y = 0.0;
		HSV.X = NAN;
		return HSV;
	}
	if (RGB.X >= max) 
		HSV.X = (RGB.Y - RGB.Z) / delta;      
	else
		if (RGB.Y >= max)
			HSV.X = 2.0 + (RGB.Z - RGB.X) / delta;
		else
			HSV.X = 4.0 + (RGB.X - RGB.Y) / delta;

	HSV.X *= 60.0;

	if (HSV.X < 0.0)
		HSV.X += 360.0;

	return HSV;
}
