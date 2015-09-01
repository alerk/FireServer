/*
 * FireDetectorThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#include "FireThread.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <stdarg.h>

#include "CommonDefine.h"

#define DELAY_TIME 60
#define RUN_CPP 1
#define MAX_CAM_DISPLAY 4

#if RUN_CPP
	#include "FireDetector/FireDetector.h"
#else
	#include "opencv2/core/core.hpp"
	#include "opencv2/imgproc/imgproc.hpp"
	#include "opencv2/highgui/highgui.hpp"

	#include "FireDetector/CommonFunctions.h"
	#include "FireDetector/STEPI_MovingRegionDetection.h"
	#include "FireDetector/STEPII_DetectionFireColoredPixels.h"
	#include "FireDetector/STEPIII_TemporalWaveletAnalysis.h"
	#include "FireDetector/STEPIV_SpatialWaveletAnalysis.h"
	#include "FireDetector/Config.h"
#endif

#include "ini_parser/iniparser.h"

//void* runFireThread(void* arg);
const bool DEBUG = true;
int num_of_source;
static int fire_instant_counter = 0;
#if RUN_CPP
	FireDetector* objFireDetector;
	cv::VideoCapture capture_;
	std::vector<FireDetector*> vectorDetector;
	const int unsigned GAP = 10;
	const int unsigned SIZE_1 = 360;
	const int unsigned SIZE_2 = 320;
	const int unsigned SIZE_3 = 280;

	unsigned char* s[MAX_CAM_DISPLAY];
	//unsigned char* s1[MAX_CAM_DISPLAY];


#else
static CvCapture* capture;
#endif

FireThread::FireThread()
{
	// TODO Auto-generated constructor stub
//	initFireThread();
	fire_instant_counter++;
	std::cout << "[FireThread]Instant: " << fire_instant_counter << std::endl;

}

FireThread::~FireThread()
{
	// TODO Auto-generated destructor stub
	pthread_cancel(fireThread);
}
#if RUN_CPP
void* FireThread::runDisplayThread(void* arg)
{
	FireThread* fireObj = (FireThread*)arg;
	int num_of_display = (num_of_source>MAX_CAM_DISPLAY)?MAX_CAM_DISPLAY:num_of_source;
	namedWindow("Display window", WINDOW_AUTOSIZE);
	while(true)
	{
		switch(num_of_display)
		{
		case 1:
			fireObj->cvShowManyImages("Display window", 640, 480, num_of_display, s[0]);
			break;
		case 2:
			fireObj->cvShowManyImages("Display window", 640, 480, num_of_display, s[0], s[1]);
			break;
		case 3:
			fireObj->cvShowManyImages("Display window", 640, 480, num_of_display, s[0], s[1], s[2]);
			break;
		case 4:
			fireObj->cvShowManyImages("Display window", 640, 480, num_of_display, s[0], s[1], s[2], s[3]);
			break;
		default:
			break;
		}
		int c = waitKey(DELAY_TIME);
		if( c == 'q' )
		{
			break;
		}
		else if(c=='f')
		{
			//(fireObj->fireDetected)(fireObj->handler);

		}
		else
		{
			//do nothing
		}
	}
	return NULL;
}
void* FireThread::runFireThread(void* arg)
{
	FireThread* fireObj = (FireThread*)arg;
	int elapsedFrame = 0;
//	Mat s[MAX_CAM_DISPLAY];


#ifdef RUN_ONE_CAM
	cv::Mat frame;
	while(true)
	{
		if(!capture_.read(frame))
		{
			std::cout << "[Run Loop]Cannot read frame" << std::endl;
		}
		int firePixel = objFireDetector->getFirePixelNumber(frame);
		if(firePixel>fireThreshold)
		{
			//count the frame that contains firePixel surpass threshold
			std::cout << elapsedFrame << " firePixel: " << firePixel << std::endl;
			elapsedFrame++;
			if(elapsedFrame > 30)//if more than sth, then fire
			{
				elapsedFrame = 0;
				(fireObj->fireDetected)(fireObj->handler);
			}
		}
		int c = waitKey(DELAY_TIME);
		if( c == 'q' )
		{
			break;
		}
		else if(c=='f')
		{
			//(fireObj->fireDetected)(fireObj->handler);

		}
		else
		{
			//do nothing
		}
//		usleep(1);
	}
	std::cout << "[FireThread]Terminate FireThread routine" << std::endl;
#else
	while(true)
	{

		bool has_image = false;
		for(int id = 0; id < num_of_source; id++)
		{
			FireDetector* temp = vectorDetector.at(id);
			if(temp->isFire)
			{
				(fireObj->fireDetected)(fireObj->handler, temp->getSourceId());
				temp->isFire = false;
			}
//			if(id<num_of_display)
//			{
//				has_image = true;
//				memset(s1, 0, 640*480*3*sizeof(unsigned char));
//				int hasNewFrame = temp->getFrame(s1[id]);
//				if(hasNewFrame)
//				{
//					memcpy(s, s1, 640*480*3*sizeof(unsigned char));
//				}
//				else
//				{
//
//				}
//				std::cout << "[FireThread]Get frame of " << temp->getSourceId() << (hasNewFrame?", new frame":", old frame") << std::endl;
////				s[id] = Mat(640, 480, CV_8UC3, buff);
//			}
		}
		//-------------ending----------------//

		usleep(DELAY_TIME);
	}
#endif
	return NULL;
}
#else
void* runFireThread(void* arg)
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
#endif

void FireThread::startFireThread()
{
	//if( pthread_create(&fireThread,NULL,run,(void*)this)!=0)
	if( pthread_create(&fireThread,NULL, FireThread::runFireThread,(void*)this)!=0) //using myCode
	{
		std::cout << "Fail to create fireThread" << std::endl;
	}

	if( pthread_create(&displayThread,NULL, FireThread::runDisplayThread,(void*)this)!=0) //using myCode
	{
		std::cout << "Fail to create displayThread" << std::endl;
	}

	for(int id = 0; id < num_of_source; id++)
	{
		FireDetector* temp = vectorDetector.at(id);
		temp->start();
	}

//	for(int id = 0; id < num_of_source; id++)
//	{
//		FireDetector* temp = vectorDetector.at(id);
//		temp->join();
//	}
	std::cout << "[FireThread]Start" << std::endl;

}

void FireThread::initFireThread()
{
	//init the video source here - prepare for multiple inputs
#ifdef RUN_ONE_CAM
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
#if RUN_CPP
	objFireDetector = new FireDetector();
	capture_ = cv::VideoCapture(src_str);
	if(!capture_.isOpened())
#else
	capture = cvCaptureFromFile(src_str.c_str());
	if(!capture)
#endif
	{
		std::cout << "Cannot open video source!" << std::endl;
	}
#else
	dictionary* ini;
	//std::string src_str[MAX_NUMBER_OF_INPUT];
	if(DEBUG)
	{
		ini = iniparser_load("Resources/FireServer.conf");
	}
	else
	{
		ini = iniparser_load("./FireServer.conf");
	}
	if(ini==NULL)
	{
		fprintf(stderr, "cannot parse file\n");
		return;
	}

	for(int id = 0; id<MAX_CAM_DISPLAY; id++)
	{
		s[id] = (unsigned char*)malloc(640*480*3*sizeof(unsigned char));
		//s1[id] = (unsigned char*)malloc(640*480*3*sizeof(unsigned char));
	}
	num_of_source = iniparser_getint(ini, "fire_detector:num_of_source",1);
	if(num_of_source>MAX_NUMBER_OF_INPUT)
	{
		num_of_source = MAX_NUMBER_OF_INPUT;
	}
	for(int i=0;i<num_of_source;i++)
	{
		char temp[10], temp_src[50], temp_threshold[30];
		sprintf(&(temp[0]), "source_%d", i);
		sprintf(&(temp_src[0]), "source_%d:source", i);
		sprintf(&(temp_threshold[0]), "source_%d:threshold", i);
		std::string source = iniparser_getstring(ini,temp_src,"Resources/usnMR6I_EAQ.mp4");
		int threshold = iniparser_getint(ini, temp_threshold, 10);
		FireDetector* fireDet = new FireDetector(i, temp, source, threshold);
		if(i<num_of_source && i<MAX_CAM_DISPLAY)
		{
			fireDet->setBuffer(s[i]);
		}
//		fireDet->join();
		vectorDetector.push_back(fireDet);
		fireDet->init();
	}
	iniparser_freedict(ini);
#endif
	std::cout << "[FireThread]Init" << std::endl;
	this->fireDetected = NULL;
}

void FireThread::joinFireThread()
{
	pthread_join(displayThread, NULL);
	pthread_join(fireThread, NULL);
}

void FireThread::connectCallback(CallbackPtr cb, void* cbHandler)
{
	fireDetected = cb;
	handler = cbHandler;
}

void FireThread::cvShowManyImages(std::string title, int s_cols, int s_rows, int nArgs,...)
{
	// img - Used for getting the arguments
	cv::Mat img;

	// [[disp_image]] - the image in which input images are to be copied
	cv::Mat disp_image;

	unsigned int size;
	unsigned int i;
	unsigned int m, n;
	unsigned int x, y;

	// w - Maximum number of images in a row
	// h - Maximum number of images in a column
	int w, h;

	// scale - How much we have to resize the image
	float scale;
	int max;

	// If the number of arguments is lesser than 0 or greater than 12
	// return without displaying
	if(nArgs <= 0)
	{
		std::cout << "Number of arguments too small...." << std::endl;
		return;
	}
	else if(nArgs > 12)
	{
		std::cout << "Number of arguments too large...." << std::endl;
		return;
	}
	// Determine the size of the image,
	// and the number of rows/cols
	// from number of arguments
	else if (nArgs == 1)
	{
		w = h = 1;
		size = SIZE_1;
	}
	else if (nArgs == 2)
	{
		w = 2; h = 1;
		size = SIZE_1;
	}
	else if (nArgs == 3 || nArgs == 4)
	{
		w = 2; h = 2;
		size = SIZE_1;
	}
	else if (nArgs == 5 || nArgs == 6)
	{
		w = 3; h = 2;
		size = SIZE_2;
	}
	else if (nArgs == 7 || nArgs == 8)
	{
		w = 4; h = 2;
		size = SIZE_2;
	}
	else
	{
		w = 4; h = 3;
		size = SIZE_3;
	}

	// Create a new 3 channel image
	disp_image = cv::Mat(cv::Size((w+1)*GAP+size*w, (h+1)*GAP+size*h), CV_8UC3, 3);

	// Used to get the arguments passed
	va_list args;
	va_start(args, nArgs);

	// Loop for nArgs number of arguments
	for (i = 0, m = GAP, n = GAP; i < nArgs; i++, m += (GAP + size))
	{
		// Get the Pointer to the IplImage
		unsigned char* temp = va_arg(args, unsigned char*);

		// Check whether it is NULL or not
		// If it is NULL, release the image, and return
		img = cv::Mat( s_rows, s_cols, CV_8UC3, temp);
		if(img.empty())
		{
			std::cout << "Invalid arguments" << std::endl;
			return;
		}

		// Find the width and height of the image
		x = img.cols;
		y = img.rows;

		// Find whether height or width is greater in order to resize the image
		max = (x > y)? x: y;

		// Find the scaling factor to resize the image
		scale = (float) ( (float) max / size );

		// Used to Align the images
		if( i % w == 0 && m!= GAP)
		{
			m = GAP;
			n+= (GAP + size);
		}

		// Set the image ROI to display the current image
		cv::Rect roi(m, n, (int)(x/scale), (int)( y/scale ));
		cv::Mat image_roi = disp_image(roi);

		// Resize the input image and copy the it to the Single Big Image
		cv::resize(img, img, cv::Size((int)(x/scale), (int)( y/scale )));
		img.copyTo(image_roi);

		// Reset the ROI in order to display the next image
	}

	// Create a new window, and show the Single Big Image
	imshow(title, disp_image);
//	updateWindow("Display window");

	// End the number of arguments
	va_end(args);
}
