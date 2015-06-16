#include "CommonFunctions.h"
#include <iostream>


#include <opencv2/imgproc/imgproc_c.h>  // Gaussian Blur
#include <opencv2/core/core_c.h>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui_c.h>  // OpenCV window I/O

float getABS(float a)
{
	if (a > 0)
		return a;
	else
		return -a;
}

IplImage * RGB_Y_convert(IplImage* RGBImage)
{
	IplImage * YUV_image;	
	IplImage * Y_image;		
	if (RGBImage == NULL)
	{
		return NULL;
	}
	YUV_image	= cvCreateImage(cvGetSize(RGBImage),8,3);
	Y_image		= cvCreateImage(cvGetSize(RGBImage),8,1);
	if (strcmp( RGBImage->colorModel,"BGR") == 0)
	{
		cvCvtColor( RGBImage, YUV_image, CV_BGR2YCrCb );
	}

	if (strcmp( RGBImage->colorModel,"RGB") == 0)
	{
		cvCvtColor( RGBImage, YUV_image, CV_RGB2YCrCb );
	}
	//take Y channel
	cvSplit(YUV_image,Y_image,NULL,NULL,NULL);
	cvReleaseImage(&YUV_image);
	return Y_image;
}
