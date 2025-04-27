#include "SampleWebPType.h"

FSampleWebpPictureInformation::FSampleWebpPictureInformation()
{
	X0 = X1 = Y0 = Y1 = 0;
}

FSampleWebpPictureInformation::FSampleWebpPictureInformation(int32 InX0, int32 InY0, int32 InX1, int32 InY1):
	X0(InX0),
	Y0(InY0),
	X1(InX1),
	Y1(InY1)
{
	
}

FSampleWebpPictureInformation::FSampleWebpPictureInformation(FSampleWebpPictureInformation& InWebpPictureInformation) :
	X0(InWebpPictureInformation.X0),
	Y0(InWebpPictureInformation.Y0),
	X1(InWebpPictureInformation.X1),
	Y1(InWebpPictureInformation.Y1)
{
}

int32 FSampleWebpPictureInformation::GetPictureWidth()
{
	return X1 - X0 + 1 > 0 ? X1 - X0 + 1 : 0;
}

int32 FSampleWebpPictureInformation::GetPictureHeight()
{
	return Y1 - Y0 + 1 > 0 ? Y1 - Y0 + 1 : 0;
}

void FSampleWebpPictureInformation::ResetPictureInformation()
{
	X0 = X1 = Y0 = Y1 = 0;
}
