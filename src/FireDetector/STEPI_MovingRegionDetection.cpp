#include "STEPI_MovingRegionDetection.h"

#include <stdio.h>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <opencv2/video/background_segm.hpp>

using namespace cv;

static cv::Mat back;
static cv::Mat front;
static cv::BackgroundSubtractorMOG2 bg;
static std::vector<std::vector<cv::Point> > contours;


//V4 with avgThreshold, the return data type is struct IplImage
void getMovingPixels_(cv::Mat input, cv::Mat& movingMap)
{
	bg.operator ()(input, front);
	bg.getBackgroundImage(back);
	//cv::erode(front,front,cv::Mat());
	//cv::dilate(front,front,cv::Mat());
	cv::findContours(front,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	vector<vector<Point> > contours_poly( contours.size() );
	for(unsigned int i = 0; i < contours.size(); i++ )
	{
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
		//						boundRect[i] = boundingRect( Mat(contours_poly[i]) );
		//						minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
	}
	for(unsigned int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( 255,255,255 );
		drawContours( front, contours_poly, i, color, CV_FILLED, 8, vector<Vec4i>(), 0, Point() );
	}
//	cv::drawContours(input,contours,-1,cv::Scalar(0,0,255),2);
	//cv::imshow("Frame",input);
//	cv::imshow("Background",back);
	GaussianBlur(front, movingMap, Size(5,5), 0, 0);
//	movingMap = front.clone();
}

void getMovingPixels(IplImage* oneDI_n, IplImage* oneDI_nm1, int lightParam, unsigned char* threshold, IplImage* setOfStationPixel)
{
	//setOfStationPixel 1: moving, 0: station
	
	/************************************************************************/
	/*                       RUN ON PRE-DEFINED HW PLATFORM                 */
	/************************************************************************/
	#ifdef  USING_HW_KERNEL
		//run with hware kernel
		printf( "old version!\n" );
	#endif

	/************************************************************************/
	/*                       RUN ON SOFTWARE PLATFORM                       */
	/************************************************************************/
	#ifdef  USING_SW_OPT
		int frameHeigh;
		int frameWidth;
		int absvalue = 0;
		float total = 0;
		int i,j;

		frameHeigh = oneDI_n->height;
		frameWidth = oneDI_n->width;

		for (i = 0; i < frameHeigh; i++)
		{
			unsigned char* ptr_Inm1 = (unsigned char*)(oneDI_nm1->imageData + i*frameWidth);
			unsigned char* ptr_In = (unsigned char*)(oneDI_n->imageData + i*frameWidth);
			unsigned char* ptr_setOfStationPixel = (unsigned char*)(setOfStationPixel->imageData + i*frameWidth);			
			for ( j = 0; j < frameWidth; j++)
			{
				absvalue = abs(ptr_In[j] - ptr_Inm1[j]);
				if ( absvalue > *threshold)
				{
					ptr_setOfStationPixel[j] = 255;				
				}
				else
				{
					ptr_setOfStationPixel[j] = 0;
				}
				//re-assign
				total += absvalue;				
			}			
		}
		total = lightParam*total / (frameWidth*frameHeigh);
		*threshold = total;		
	#endif
}

