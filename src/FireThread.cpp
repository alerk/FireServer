/*
 * FireDetectorThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#include "FireThread.h"
#include <iostream>
#include <unistd.h>

//#include "opencv2/core/core_c.h"
//#include "opencv2/imgproc/imgproc_c.h"
//#include "opencv2/highgui/highgui_c.h"

#include "FireDetector/CommonFunctions.h"
#include "FireDetector/STEPI_MovingRegionDetection.h"
#include "FireDetector/STEPII_DetectionFireColoredPixels.h"
#include "FireDetector/STEPIII_TemporalWaveletAnalysis.h"
#include "FireDetector/STEPIV_SpatialWaveletAnalysis.h"
#include "FireDetector/Config.h"
#include "ini_parser/iniparser.h"

#include <tr1/functional>


static void* run(void* arg);
static CvCapture* capture;
static int fireThreshold=0;
FireThread::FireThread()
{
	// TODO Auto-generated constructor stub

}

FireThread::~FireThread()
{
	// TODO Auto-generated destructor stub
}

static void* run_(void* arg)
{
	FireThread* fireObj = (FireThread*)arg;
	// create a buff with pointer link
	cv::Mat frame;
	cv::Mat ptrFrameBuffer[FRAME_BUFFER_SIZE];
	cv::Mat movingMap;
	cv::Mat MaskOfIdxFirePoints;
	cv::Mat setOfFlickerPoints;
	cv::Mat MaskOfIdxFireColoredPoints;
	cv::Mat temp;
	cv::Mat Y_image;
	cv::Mat test_image;
	char c;
	int i,j;
	int convertColorCode = 0;
	int frameWidth, frameHeigh;
	unsigned char MovingThreshold = 0;
	float row_avg = 0;
	float radius_row = 0;
	float col_avg = 0;
	float radius_col = 0;
	int countTemp = 0;
	int window_size_row_min = 0;
	int window_size_row_max = 0;
	int window_size_col_min = 0;
	int window_size_col_max = 0;
}

static void* run(void* arg)
{
	FireThread* fireObj = (FireThread*)arg;
	// create a buff with pointer link
	IplImage* frame;
	IplImage* ptrFrameBuffer[FRAME_BUFFER_SIZE];
	IplImage* movingMap;
	IplImage* MaskOfIdxFirePoints;
	IplImage* setOfFlickerPoints;
	IplImage* MaskOfIdxFireColoredPoints;
	IplImage* temp;
	IplImage* Y_image;
	IplImage* test_image;
	char c;
	int i,j;
	int convertColorCode = 0;
	int frameWidth, frameHeigh;
	unsigned char MovingThreshold = 0;
	float row_avg = 0;
	float radius_row = 0;
	float col_avg = 0;
	float radius_col = 0;
	int countTemp = 0;
	int window_size_row_min = 0;
	int window_size_row_max = 0;
	int window_size_col_min = 0;
	int window_size_col_max = 0;

	//	    char cCurrentPath[100];
	//	    if(!getcwd(cCurrentPath,100))
		//	    {
		//	    	return NULL;
	//	    }
	//	    cCurrentPath[99]='\0';
	//	    std::cout << cCurrentPath << std::endl;

	cvNamedWindow("video-window",1);
#ifdef TEST_STEP1
	cvNamedWindow("MovingPixel",1);
#endif
#ifdef TEST_STEP2
	cvNamedWindow("FireColoredPixel",1);
#endif
#ifdef TEST_STEP3
	cvNamedWindow("FlickerPixel",1);
#endif
#ifdef TEST_STEP4
	cvNamedWindow("FiredPixel",1);
#endif

	//init STEP2 GAUSS
	createMixtuaGaussModelOfFireSamples(INPUT_IMAGES_SAMPLES_MODE,
			FIRE_FILE0,
			FIRE_FILE1,
			FIRE_FILE2,
			FIRE_FILE3,
			FIRE_FILE4,
			FIRE_FILE5,
			FIRE_FILE6,
			FIRE_FILE7,
			FIRE_FILE8,
			FIRE_FILE9);

	//	    capture = cvCaptureFromAVI( TEST_FILE);
	if(!capture){
		return NULL;
	}

	frame = cvQueryFrame( capture );
	CvSize frameSize = cvGetSize(frame);

	frameWidth = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	frameHeigh = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	// init mask of fired pixel
	temp = cvCreateImage(cvSize(frameWidth, frameHeigh),IPL_DEPTH_8U,3);
	test_image = cvCreateImage(cvSize(frameWidth, frameHeigh),IPL_DEPTH_8U,3);
	movingMap = cvCreateImage(cvSize(frameWidth, frameHeigh),IPL_DEPTH_8U,1);
	setOfFlickerPoints = cvCreateImage(cvSize(frameWidth, frameHeigh),IPL_DEPTH_8U,1);
	MaskOfIdxFireColoredPoints = cvCreateImage(cvSize(frameWidth, frameHeigh),IPL_DEPTH_8U,1);
	MaskOfIdxFirePoints = cvCreateImage(cvSize(frameWidth, frameHeigh),IPL_DEPTH_8U,1);
	// init frame-buffer of Y channels

	if (  strcmp("RGB",frame->colorModel)==0)
	{
		convertColorCode = CV_RGB2YCrCb;
	}
	if (strcmp("BGR",frame->colorModel)==0)
	{
		convertColorCode = CV_BGR2YCrCb;
	}

	for (i = 0; i < FRAME_BUFFER_SIZE; i ++)
	{
		frame = cvQueryFrame( capture );
		cvCvtColor(frame,temp,convertColorCode);
		Y_image = cvCreateImage(cvSize(frameWidth, frameHeigh),IPL_DEPTH_8U,1);
		cvSplit(temp,Y_image,NULL,NULL,NULL);
		ptrFrameBuffer[i] = cvCloneImage(Y_image);
		cvReleaseImage(&Y_image);
	}
	while (true)
	{
		frame = cvQueryFrame( capture );
		if (!frame)
		{
			break;
		}
		/**********************do with frame buffer below this line**********/
		// remove frame at 1st place
		cvReleaseImage(&ptrFrameBuffer[0]);
		// shift other items 1 step toward 1st place
		for (i = 0; i < FRAME_BUFFER_SIZE-1; i++)
		{
			ptrFrameBuffer[i] = ptrFrameBuffer[i+1];
		}
		// copy newest frame to the last place
		cvCvtColor(frame,temp,convertColorCode);
		Y_image = cvCreateImage(cvSize(frameWidth, frameHeigh),IPL_DEPTH_8U,1);
		cvSplit(temp,Y_image,NULL,NULL,NULL);
		ptrFrameBuffer[FRAME_BUFFER_SIZE-1] = cvCloneImage(Y_image);
		cvReleaseImage(&Y_image);
		/**********************do with frame buffer above this line**********/

		/************************************************************************/
		/*                APPLY STEP1 BELOW THIS COMMENT                        */
		/************************************************************************/
		getMovingPixels(ptrFrameBuffer[FRAME_BUFFER_SIZE-1],
				ptrFrameBuffer[FRAME_BUFFER_SIZE-2],
				STEPI_LIGHT_PARAM,
				&MovingThreshold, movingMap);

#ifdef TEST_STEP1
		test_image = cvCloneImage(frame);
		for (i = 0; i < frame->height; i++)
		{
			unsigned char* ptr_setOfStationPixel = (unsigned char*)(movingMap->imageData + i*frame->width);
			for ( j = 0; j < frame->width; j++)
			{
				if ( ptr_setOfStationPixel[j] == 255)
				{
					cvRectangle(test_image,cvPoint(j-4,i-4),cvPoint(j+4,i+4),cvScalar(0,255,0,0),1,8,0);
				}
			}
		}
		cvShowImage("MovingPixel",movingMap);
		cvShowImage("video-window",test_image);
#endif
/*************************END STEP1**************************************/
		//@@@@@@@@@@@@@@@@
		//@@@@@@@@@@@
		//@@@@@@
		/************************************************************************/
		/*                APPLY STEP2 BELOW THIS COMMENT                        */
		/************************************************************************/
		setOfColorPixel( movingMap,
				MaskOfIdxFireColoredPoints,
				frame);
#ifdef TEST_STEP2
		test_image = cvCloneImage(frame);
		for (i = 0; i < frame->height; i++)
		{
			unsigned char* ptr_MaskOfIdxFireColoredPoints = (unsigned char*)(MaskOfIdxFireColoredPoints->imageData + i*frame->width);
			for ( j = 0; j < frame->width; j++)
			{
				if ( ptr_MaskOfIdxFireColoredPoints[j] == 255)
				{
					cvRectangle(test_image,cvPoint(j-4,i-4),cvPoint(j+4,i+4),cvScalar(0,255,0,0),1,8,0);
				}
			}
		}
		cvShowImage("FireColoredPixel",MaskOfIdxFireColoredPoints);
		cvShowImage("video-window",test_image);
#endif
/*************************END STEP2**************************************/
		//@@@@@@@@@@@@@@@@
		//@@@@@@@@@@@
		//@@@@@@
		/************************************************************************/
		/*                APPLY STEP3 BELOW THIS COMMENT                        */
		/************************************************************************/
		getFlickerPixel(ptrFrameBuffer, frameHeigh, frameWidth, MaskOfIdxFireColoredPoints,setOfFlickerPoints);
#ifdef TEST_STEP3
		test_image = cvCloneImage(frame);
		for (i = 0; i < frame->height; i++)
		{
			unsigned char* ptr_setOfFlickerPoints = (unsigned char*)(setOfFlickerPoints->imageData + i*frame->width);
			for ( j = 0; j < frame->width; j++)
			{
				if ( ptr_setOfFlickerPoints[j] == 255)
				{
					cvRectangle(test_image,cvPoint(j-4,i-4),cvPoint(j+4,i+4),cvScalar(0,255,0,0),1,8,0);
				}
			}
		}

		cvShowImage("FlickerPixel",setOfFlickerPoints);
		cvShowImage("video-window",test_image);
#endif
/*************************END STEP3**************************************/
		//@@@@@@@@@@@@@@@@
		//@@@@@@@@@@@
		//@@@@@@
		/************************************************************************/
		/*                APPLY STEP4 BELOW THIS COMMENT                        */
		/************************************************************************/

		STEPIV_SpactionVariance(ptrFrameBuffer[FRAME_BUFFER_SIZE-1],
				setOfFlickerPoints,
				MaskOfIdxFirePoints);
#ifdef TEST_STEP4
		test_image = cvCloneImage(frame);
		for (i = 0; i < frame->height; i++)
		{
			unsigned char* ptr_MaskOfIdxFirePoints = (unsigned char*)(MaskOfIdxFirePoints->imageData + i*frame->width);
			for ( j = 0; j < frame->width; j++)
			{
				if ( ptr_MaskOfIdxFirePoints[j] == 255)
				{
					cvRectangle(test_image,cvPoint(j-4,i-4),cvPoint(j+4,i+4),cvScalar(0,255,0,0),1,8,0);
				}
			}
		}

		cvShowImage("FiredPixel",MaskOfIdxFirePoints);
		cvShowImage("video-window",test_image);
		int value = cvCountNonZero(MaskOfIdxFirePoints);
		if(value > fireThreshold)
		{
			(fireObj->fireDetected)(fireObj->handler);
		}
#endif
/*************************END STEP4**************************************/
		//@@@@@@@@@@@@@@@@
		//@@@@@@@@@@@
		//@@@@@@
		/************************************************************************/

		/************************************************************************/
		/*                APPLY STEP5 BELOW THIS COMMENT                        */
		/************************************************************************/
		// The main function of filter at step 5 is a predefined bg mask. It eliminates every defined area in frame that assumed to be a background
		// - cac diem nhap nhay tuan hoan mang tinh quy luat
		// - cac diem khong the nao chay duoc :))
		// - cac diem sang biet chac khong the nao la lua ma de bi nham sang lua :))
		/*************************END STEP5**************************************/
		//@@@@@@@@@@@@@@@@
		//@@@@@@@@@@@
		//@@@@@@
		/************************************************************************/
		/*                [FOR TEST ONLY] DISPLAY RESULT                        */
		/************************************************************************/
		/*************************[FOR TEST ONLY] END DISPLAY********************/

		c = cvWaitKey(30);
		if( c == 'c' )
		{
			break;
		}
		else if(c=='f')
		{
			(fireObj->fireDetected)(fireObj->handler);

		}
		else
		{
			//do nothing
		}
	}

	//	while(true)
	//	{
	//		(fireObj->fireDetected)(fireObj->handler);
	//		sleep(1);
	//	}
	return NULL;
}

void FireThread::startFireThread()
{
	if( pthread_create(&fireThread,NULL,run,(void*)this)!=0)
	{
		std::cout << "Fail to create fireThread" << std::endl;
	}
}

void FireThread::initFireThread()
{
	//init the video source here - prepare for multiple inputs
	dictionary* ini;
	std::string src_str;
	ini = iniparser_load("Resources/FireServer.conf");
	if(ini==NULL)
	{
		fprintf(stderr, "cannot parse file\n");
		return;
	}
	//    iniparser_dump(ini, stderr);
	src_str = iniparser_getstring(ini,"fire_detector:source", "Resources/outdoor_daytime_10m_heptane_CCD_001.avi");
	fireThreshold = iniparser_getint(ini, "fire_detector:threshold",10);
	iniparser_freedict(ini);
	capture = cvCaptureFromFile(src_str.c_str());
	if(!capture)
	{
		std::cout << "Cannot open video source!" << std::endl;
	}
	this->fireDetected = NULL;

}

void FireThread::joinFireThread()
{
	pthread_join(fireThread, NULL);
}

void FireThread::connectCallback(CallbackPtr cb, void* cbHandler)
{
	fireDetected = cb;
	handler = cbHandler;
}
