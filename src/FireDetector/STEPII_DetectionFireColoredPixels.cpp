#include "STEPII_DetectionFireColoredPixels.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O

using namespace cv;
GaussOfFireSamples arrayFireSamples[NUMBER_OF_FIRE_SAMPLES];



void createMixtuaGaussModelOfFireSamples_(int mode, char * fireName1,
	char * fireName2,
	char * fireName3,
	char * fireName4,
	char * fireName5,
	char * fireName6,
	char * fireName7,
	char * fireName8,
	char * fireName9,
	char * fireName10)
{
	int i,j,x,y;
	float R_avg		;
	float G_avg		;
	float B_avg		;
	double R_radius	;
	double G_radius	;
	double B_radius	;
	int numberOfPixel;
	cv::Mat img[NUMBER_OF_FIRE_SAMPLES];
	if (mode == INPUT_IMAGES_SAMPLES_MODE)
	{

		//read the input file
		img[0] = imread(fireName1);
		img[1] = imread(fireName2);
		img[2] = imread(fireName3);
		img[3] = imread(fireName4);
		img[4] = imread(fireName5);
		img[5] = imread(fireName6);
		img[6] = imread(fireName7);
		img[7] = imread(fireName8);
		img[8] = imread(fireName9);
		img[9] = imread(fireName10);

		for ( i = 0; i < NUMBER_OF_FIRE_SAMPLES; i++)
		{
			if (!img[i].empty())
			{
				//calculate Gauss for each sphere sample
				R_avg			= 0;
				G_avg			= 0;
				B_avg			= 0;
				R_radius		= 0;
				G_radius		= 0;
				B_radius		= 0;
				//cv::Mat::MSize imgSize = img[i].size;
				int imwidth = img[i].cols;
				int imheight = img[i].rows;
				numberOfPixel	= imwidth * imheight;
				int cn = img[i].channels();
				Scalar_<uint8_t> rgbPixel;
				for ( x = 0; x < imheight; x++ )
				{
					uchar* row_ptr = img[i].row(x).data;

					for ( y = 0; y < imwidth; y++)
					{
						rgbPixel.val[0] = row_ptr[y*cn+0];
						rgbPixel.val[1] = row_ptr[y*cn+1];
						rgbPixel.val[2] = row_ptr[y*cn+2];

						R_avg+=rgbPixel.val[0];
						G_avg+=rgbPixel.val[1];
						B_avg+=rgbPixel.val[2];
					}
				}
				R_avg /= numberOfPixel;
				G_avg /= numberOfPixel;
				B_avg /= numberOfPixel;

				for ( x = 0; x < imheight; x++ )
				{
					uchar* row_ptr = img[i].row(x).data;
					for ( y = 0; y < imwidth; y++)
					{
						rgbPixel.val[0] = row_ptr[y*cn+0];
						rgbPixel.val[1] = row_ptr[y*cn+1];
						rgbPixel.val[2] = row_ptr[y*cn+2];

						R_radius += (rgbPixel.val[0]-R_avg)*(rgbPixel.val[0]-R_avg);
						G_radius += (rgbPixel.val[1]-G_avg)*(rgbPixel.val[1]-G_avg);
						B_radius += (rgbPixel.val[2]-B_avg)*(rgbPixel.val[2]-B_avg);
					}
				}

				R_radius /=numberOfPixel;
				G_radius /=numberOfPixel;
				B_radius /=numberOfPixel;

				R_radius = sqrt(R_radius);
				G_radius = sqrt(G_radius);
				B_radius = sqrt(B_radius);

				arrayFireSamples[i].R_center = R_avg;
				arrayFireSamples[i].G_center = G_avg;
				arrayFireSamples[i].B_center = B_avg;
				arrayFireSamples[i].R_radius = R_radius;
				arrayFireSamples[i].G_radius = G_radius;
				arrayFireSamples[i].B_radius = B_radius;
			}
		}
	}

}

void createMixtuaGaussModelOfFireSamples(int mode, char * fireName1,
	char * fireName2,
	char * fireName3,
	char * fireName4,
	char * fireName5,
	char * fireName6,
	char * fireName7,
	char * fireName8,
	char * fireName9,
	char * fireName10)
{
	int i,j,x,y;
	float R_avg		;
	float G_avg		;
	float B_avg		;
	double R_radius	;
	double G_radius	;
	double B_radius	;
	int numberOfPixel;
	IplImage* img[NUMBER_OF_FIRE_SAMPLES];
	if (mode == INPUT_IMAGES_SAMPLES_MODE)
	{
		img[0] = cvLoadImage(fireName1,1);
		img[1] = cvLoadImage(fireName2,1);
		img[2] = cvLoadImage(fireName3,1);
		img[3] = cvLoadImage(fireName4,1);
		img[4] = cvLoadImage(fireName5,1);
		img[5] = cvLoadImage(fireName6,1);
		img[6] = cvLoadImage(fireName7,1);
		img[7] = cvLoadImage(fireName8,1);
		img[8] = cvLoadImage(fireName9,1);
		img[9] = cvLoadImage(fireName10,1);

		for ( i = 0; i < NUMBER_OF_FIRE_SAMPLES; i++)
		{
			if (img[i] != NULL)
			{
				//calculate Gauss for each sphere sample
				R_avg			= 0;
				G_avg			= 0;
				B_avg			= 0;
				R_radius		= 0;
				G_radius		= 0;
				B_radius		= 0;
				numberOfPixel	= img[i]->height * img[i]->width; 

				for ( x = 0; x < img[i]->height; x++ )
				{
					unsigned char * ptr = (unsigned char*)(img[i]->imageData + x*img[i]->widthStep);
					for ( y = 0; y < img[i]->width; y++)
					{
						R_avg += ptr[3*y];
						G_avg += ptr[3*y+1];
						B_avg += ptr[3*y+2];
					}
				}
				R_avg /= numberOfPixel;
				G_avg /= numberOfPixel;
				B_avg /= numberOfPixel;

				for ( x = 0; x < img[i]->height; x++ )
				{
					unsigned char * ptr = (unsigned char*)(img[i]->imageData + x*img[i]->widthStep);
					for ( y = 0; y < img[i]->width; y++)
					{
						R_radius += (ptr[3*y] - R_avg)*(ptr[3*y] - R_avg);
						G_radius += (ptr[3*y+1] - G_avg)*(ptr[3*y+1] - G_avg);
						B_radius += (ptr[3*y+2] - B_avg)*(ptr[3*y+2] - B_avg);
					}
				}
				R_radius /=numberOfPixel;
				G_radius /=numberOfPixel;
				B_radius /=numberOfPixel;

				R_radius = sqrt(R_radius);
				G_radius = sqrt(G_radius);
				B_radius = sqrt(B_radius);

				arrayFireSamples[i].R_center = R_avg;
				arrayFireSamples[i].G_center = G_avg;
				arrayFireSamples[i].B_center = B_avg;
				arrayFireSamples[i].R_radius = R_radius;
				arrayFireSamples[i].G_radius = G_radius;
				arrayFireSamples[i].B_radius = B_radius;
			}

		}

		for ( i = 0; i< NUMBER_OF_FIRE_SAMPLES; i++)
		{
			cvReleaseImage(&img[i]);
		}
	} 

	if(mode == CONFIGED_NUMBER_MODE)
	{
		arrayFireSamples[0].R_center = FIRE_COLOR_DIS1_MEAN_RED;
		arrayFireSamples[0].G_center = FIRE_COLOR_DIS1_MEAN_GREEN;
		arrayFireSamples[0].B_center = FIRE_COLOR_DIS1_MEAN_BLUE;
		arrayFireSamples[0].R_radius = FIRE_COLOR_DIS1_VARIANCE;
		arrayFireSamples[0].G_radius = FIRE_COLOR_DIS1_VARIANCE;
		arrayFireSamples[0].B_radius = FIRE_COLOR_DIS1_VARIANCE;

		arrayFireSamples[1].R_center = FIRE_COLOR_DIS2_MEAN_RED;
		arrayFireSamples[1].G_center = FIRE_COLOR_DIS2_MEAN_GREEN;
		arrayFireSamples[1].B_center = FIRE_COLOR_DIS2_MEAN_BLUE;
		arrayFireSamples[1].R_radius = FIRE_COLOR_DIS2_VARIANCE;
		arrayFireSamples[1].G_radius = FIRE_COLOR_DIS2_VARIANCE;
		arrayFireSamples[1].B_radius = FIRE_COLOR_DIS2_VARIANCE;

		arrayFireSamples[2].R_center = FIRE_COLOR_DIS3_MEAN_RED;
		arrayFireSamples[2].G_center = FIRE_COLOR_DIS3_MEAN_GREEN;
		arrayFireSamples[2].B_center = FIRE_COLOR_DIS3_MEAN_BLUE;
		arrayFireSamples[2].R_radius = FIRE_COLOR_DIS3_VARIANCE;
		arrayFireSamples[2].G_radius = FIRE_COLOR_DIS3_VARIANCE;
		arrayFireSamples[2].B_radius = FIRE_COLOR_DIS3_VARIANCE;

		arrayFireSamples[3].R_center = FIRE_COLOR_DIS4_MEAN_RED;
		arrayFireSamples[3].G_center = FIRE_COLOR_DIS4_MEAN_GREEN;
		arrayFireSamples[3].B_center = FIRE_COLOR_DIS4_MEAN_BLUE;
		arrayFireSamples[3].R_radius = FIRE_COLOR_DIS4_VARIANCE;
		arrayFireSamples[3].G_radius = FIRE_COLOR_DIS4_VARIANCE;
		arrayFireSamples[3].B_radius = FIRE_COLOR_DIS4_VARIANCE;

		arrayFireSamples[4].R_center = FIRE_COLOR_DIS5_MEAN_RED;
		arrayFireSamples[4].G_center = FIRE_COLOR_DIS5_MEAN_GREEN;
		arrayFireSamples[4].B_center = FIRE_COLOR_DIS5_MEAN_BLUE;
		arrayFireSamples[4].R_radius = FIRE_COLOR_DIS5_VARIANCE;
		arrayFireSamples[4].G_radius = FIRE_COLOR_DIS5_VARIANCE;
		arrayFireSamples[4].B_radius = FIRE_COLOR_DIS5_VARIANCE;

		arrayFireSamples[5].R_center = FIRE_COLOR_DIS6_MEAN_RED;
		arrayFireSamples[5].G_center = FIRE_COLOR_DIS6_MEAN_GREEN;
		arrayFireSamples[5].B_center = FIRE_COLOR_DIS6_MEAN_BLUE;
		arrayFireSamples[5].R_radius = FIRE_COLOR_DIS6_VARIANCE;
		arrayFireSamples[5].G_radius = FIRE_COLOR_DIS6_VARIANCE;
		arrayFireSamples[5].B_radius = FIRE_COLOR_DIS6_VARIANCE;

		arrayFireSamples[6].R_center = FIRE_COLOR_DIS7_MEAN_RED;
		arrayFireSamples[6].G_center = FIRE_COLOR_DIS7_MEAN_GREEN;
		arrayFireSamples[6].B_center = FIRE_COLOR_DIS7_MEAN_BLUE;
		arrayFireSamples[6].R_radius = FIRE_COLOR_DIS7_VARIANCE;
		arrayFireSamples[6].G_radius = FIRE_COLOR_DIS7_VARIANCE;
		arrayFireSamples[6].B_radius = FIRE_COLOR_DIS7_VARIANCE;

		arrayFireSamples[7].R_center = FIRE_COLOR_DIS8_MEAN_RED;
		arrayFireSamples[7].G_center = FIRE_COLOR_DIS8_MEAN_GREEN;
		arrayFireSamples[7].B_center = FIRE_COLOR_DIS8_MEAN_BLUE;
		arrayFireSamples[7].R_radius = FIRE_COLOR_DIS8_VARIANCE;
		arrayFireSamples[7].G_radius = FIRE_COLOR_DIS8_VARIANCE;
		arrayFireSamples[7].B_radius = FIRE_COLOR_DIS8_VARIANCE;

		arrayFireSamples[8].R_center = FIRE_COLOR_DIS9_MEAN_RED;
		arrayFireSamples[8].G_center = FIRE_COLOR_DIS9_MEAN_GREEN;
		arrayFireSamples[8].B_center = FIRE_COLOR_DIS9_MEAN_BLUE;
		arrayFireSamples[8].R_radius = FIRE_COLOR_DIS9_VARIANCE;
		arrayFireSamples[8].G_radius = FIRE_COLOR_DIS9_VARIANCE;
		arrayFireSamples[8].B_radius = FIRE_COLOR_DIS9_VARIANCE;

		arrayFireSamples[9].R_center = FIRE_COLOR_DIS10_MEAN_RED;
		arrayFireSamples[9].G_center = FIRE_COLOR_DIS10_MEAN_GREEN;
		arrayFireSamples[9].B_center = FIRE_COLOR_DIS10_MEAN_BLUE;
		arrayFireSamples[9].R_radius = FIRE_COLOR_DIS10_VARIANCE;
		arrayFireSamples[9].G_radius = FIRE_COLOR_DIS10_VARIANCE;
		arrayFireSamples[9].B_radius = FIRE_COLOR_DIS10_VARIANCE;
	}
}
int isFireColored_()
{
	return 0;
}

void setOfColorPixel(cv::Mat movingPixels,cv::Mat& MaskOfIdxFirePoints, cv::Mat RGBcurrentframe)
{
	//change rgb to YCbCr
	Mat YCrCbFrame;
	RGBcurrentframe.convertTo(YCrCbFrame, CV_32FC3, 1/255.0);
	cvtColor(YCrCbFrame, YCrCbFrame, CV_BGR2YCrCb);
	//			src			dest		type


	//calculate the mean of Cb, Cr, Y
	//extract Y, Cr, Cb channel
//	Mat YChannel(YCrCbFrame.rows, YCrCbFrame.cols, CV_8UC1);
//
//	Mat CrChannel(YCrCbFrame.rows, YCrCbFrame.cols, CV_8UC1);
//
//	Mat CbChannel(YCrCbFrame.rows, YCrCbFrame.cols, CV_8UC1);
//
//	Mat out[] = {YChannel, CrChannel, CbChannel};
//	// Ycc[0] -> Y; Ycc[1] -> Cr; Ycc[2] -> Cb
//	int from_to[] = {0,0,1,1,2,2};
//	mixChannels(&YCrCbFrame, 1, out, 3, from_to, 3);
	Mat YChannel, CrChannel, CbChannel;
	vector<Mat> channels(3);
	split(YCrCbFrame, channels);
	YChannel = channels[0];
	CrChannel = channels[1];
	CbChannel = channels[2];



	//set the MaskOfIdxFirePoint

	//combine 2 Mat to form Fire Color Moving

	//result at  this point, do not wait to the 3 and 4 step
}

int isFireColored(unsigned char R_value,unsigned char G_value,unsigned char B_value)
{
	int i,j;
	float R_center	;
	float G_center	;
	float B_center	;
	double R_radius	;
	double G_radius	;
	double B_radius	;
	float r			;
	float g			;
	float b			;
	//1. check the fire colored pixels
	for ( i = 0; i < NUMBER_OF_FIRE_SAMPLES; i++)
	{
		R_center		= arrayFireSamples[i].R_center;
		G_center		= arrayFireSamples[i].G_center;
		B_center		= arrayFireSamples[i].B_center;
		R_radius		= arrayFireSamples[i].R_radius;
		G_radius		= arrayFireSamples[i].G_radius;
		B_radius		= arrayFireSamples[i].B_radius;
		r				= abs(R_value-R_center);
		g				= abs(G_value-G_center);
		b				= abs(B_value-B_center);
		if (r<= (R_radius) &&
			g<= (G_radius) &&
			b<= (B_radius) )
		{
			// this is a fire colored pixel
			return 1;
		}
	}
	return 0;
}

//void setOfColorPixel(unsigned char* movingPixels,unsigned char* MaskOfIdxFirePoints, IplImage* RGBcurrentframe,int grayWidthStep)
//{
//	int i = 0;
//	int j = 0;
//	int frame_heigh = RGBcurrentframe->height;
//	int frame_width = RGBcurrentframe->width;
//	for ( i = 0;i < frame_heigh;i++)
//	{
//		uchar* ptr = (uchar*)(RGBcurrentframe->imageData + i*RGBcurrentframe->widthStep);
//		unsigned char* ptrMovingPoint = (unsigned char*)(movingPixels + i * grayWidthStep);
//		unsigned char* ptrMaskOfIdxFirePoints = (unsigned char*)(MaskOfIdxFirePoints + i * grayWidthStep);
//		for ( j = 0; j< frame_width;j++)
//		{
//
//			if ( strcmp("RGB",RGBcurrentframe->colorModel)==0 
//				&& isFireColored(ptr[3*j],ptr[3*j+1],ptr[3*j+2]) == 0 
//				&& ptrMovingPoint[j] == 1)
//			{
//				//pixel is moving but not a fire-colorer, take it out of set
//				ptrMovingPoint[j] = 0;
//			}
//
//			if ( strcmp("BGR",RGBcurrentframe->colorModel)==0 
//				&& isFireColored(ptr[3*j+2],ptr[3*j+1],ptr[3*j]) == 0 
//				&& ptrMovingPoint[j] == 1)
//			{
//				//pixel is moving but not a fire-colorer, take it out of set
//				ptrMovingPoint[j] = 0;
//			}
//
//			if (ptrMovingPoint[j] == 1)// mask pixel moves
//			{
//				ptrMaskOfIdxFirePoints[j] = 1;
//			}
//		}
//	}
//}




void setOfColorPixel(IplImage* movingPixels,IplImage* MaskOfIdxFirePoints, IplImage* RGBcurrentframe)
{
	int i = 0;
	int j = 0;
	int frame_heigh = RGBcurrentframe->height;
	int frame_width = RGBcurrentframe->width;
	for ( i = 0;i < frame_heigh;i++)
	{
		unsigned char* ptr = (unsigned char*)(RGBcurrentframe->imageData + i*RGBcurrentframe->widthStep);
		unsigned char* ptrMovingPoint = (unsigned char*)(movingPixels->imageData + i * movingPixels->widthStep);
		unsigned char* ptrMaskOfIdxFirePoints = (unsigned char*)(MaskOfIdxFirePoints->imageData + i * MaskOfIdxFirePoints->widthStep);
		for ( j = 0; j< frame_width;j++)
		{

			if ( strcmp("RGB",RGBcurrentframe->colorModel)==0 
				&& isFireColored(ptr[3*j],ptr[3*j+1],ptr[3*j+2]) == 0 
				&& ptrMovingPoint[j] == 255)
			{
				//pixel is moving but not a fire-colorer, take it out of set
				ptrMovingPoint[j] = 0;
			}

			if ( strcmp("BGR",RGBcurrentframe->colorModel)==0 
				&& isFireColored(ptr[3*j+2],ptr[3*j+1],ptr[3*j]) == 0 
				&& ptrMovingPoint[j] == 255)
			{
				//pixel is moving but not a fire-colorer, take it out of set
				ptrMovingPoint[j] = 0;
			}
			
				ptrMaskOfIdxFirePoints[j] = ptrMovingPoint[j] ;			
		}
	}
}

