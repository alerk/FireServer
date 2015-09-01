/*
 * FireDetector.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: nguyen
 */

#include "FireDetector.h"
#include <stdarg.h>
#include <iostream>
#include <stdio.h>
#include <queue>
#include  <ctime>



#define DELAY_TIME_MS 3
#define DUAL_THREAD 0

//void* run(void* arg);
std::queue<unsigned char*> frameQueue;

static int detector_instant = 0;
const int GAP = 10;
const int SIZE_1 = 360;
const int SIZE_2 = 320;
const int SIZE_3 = 280;

FireDetector::FireDetector(int id, std::string name, std::string input, int threshold) {
	sourceId = id;
	sourceName = name;
	sourceVideo = input;
	fireThreshold = threshold;

	char wName[25];
	sprintf(&(wName[0]),"Frames_%s", sourceName.c_str());
	std::cout << wName << std::endl;
	//namedWindow(wName);
	imgWidth = imgHeight = -1;

//	init();
//	start();
	//join();
	detector_instant++;
	std::cout << "[FireDetector]Instant: " << detector_instant << std::endl;

}

FireDetector::~FireDetector()
{
	// TODO Auto-generated destructor stub
}

int FireDetector::getFirePixelNumber(BackgroundSubtractorMOG2 bg, Mat frame, Mat back, Mat front) {
	std::vector<std::vector<cv::Point> > contours;

		Mat YCrCbFrame;
		Mat YChannel, CrChannel, CbChannel;
		Mat Y_Cb, Cr_Cb;
		Mat colorMask;

		//check for input frame
		if(frame.empty())
		{
			return -1;
		}
		//---------------detect moving pixel------------//
		//       using BackgroundSubstractMOG2 			//
		//----------------------------------------------//
		bg.operator ()(frame, front);
		bg.getBackgroundImage(back);
		//cv::erode(front,front, cv::Mat());
		//cv::dilate(front, front, cv::Mat());
		cv::medianBlur(front, front, 5);
		cv::findContours(front,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		vector<vector<Point> > contours_poly( contours.size() );
		vector<Rect> boundRect( contours.size() );
		vector<Point2f>center( contours.size() );
		vector<float>radius( contours.size() );

		for(unsigned int i = 0; i < contours.size(); i++ )
		{
			approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
			boundRect[i] = boundingRect( Mat(contours_poly[i]) );
			minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
		}

		for(unsigned int i = 0; i< contours.size(); i++ )
		{
			Scalar color = Scalar( 255,255,255 );
			//params:	  input   output       contourIdx	color    thickness
			drawContours( front, contours_poly,    i,       color,   CV_FILLED, 8, vector<Vec4i>(), 0, Point() );
		}

		//----detect fire color----//
		//-------------------------------------------------------------------//
		//	pixel = fire color when											 //
		//			valueY > valueCb &&										 //
		//			valueCr > valueCb &&									 //
		//			(valueY > meanY && valueCr > meanCr && valueCb < meanCb) //
		//-------------------------------------------------------------------//

		//get YCrCb channel
		cvtColor(frame, YCrCbFrame, CV_BGR2YCrCb);
		vector<Mat> channels(3);
		split(YCrCbFrame, channels);
		YChannel = channels[0];
		CrChannel = channels[1];
		CbChannel = channels[2];

		//calculate mean of 3 channels: => for further use
	//	unsigned char Y_mean, Cr_mean, Cb_mean;
	//	Y_mean = (unsigned char)mean(YChannel)[0];
	//	Cr_mean = (unsigned char)mean(CrChannel)[0];
	//	Cb_mean = (unsigned char)mean(CbChannel)[0];

		colorMask = Mat(frame.rows, frame.cols, CV_8UC1);
		Y_Cb  = Mat(frame.rows, frame.cols, CV_8UC1);//YChannel minus CbChannel
		Cr_Cb = Mat(frame.rows, frame.cols, CV_8UC1);//CrChannel minus CbChannel
		subtract(YChannel, CbChannel, Y_Cb); threshold(Y_Cb, Y_Cb, 10, 255, THRESH_BINARY);
		subtract(CrChannel, CbChannel, Cr_Cb);threshold(Cr_Cb, Cr_Cb, 10, 255, THRESH_BINARY);

		//colorMask = front & Y_Cb & Y_Cr
		bitwise_and(front, Y_Cb, colorMask);
		bitwise_and(colorMask, Cr_Cb, colorMask);

		int fireCount = countNonZero(colorMask);

		cvtColor(front, front, CV_GRAY2BGR);
		cvtColor(Y_Cb, Y_Cb, CV_GRAY2BGR);
		cvtColor(Cr_Cb, Cr_Cb, CV_GRAY2BGR);
		cvtColor(colorMask, colorMask, CV_GRAY2BGR);

		char wName[25];
		sprintf(&(wName[0]),"Frames_%s", sourceName.c_str());
//		std::cout << "[FireDetector " << sourceName << "]Send display!" <<std::endl;
//		display->enqueue((unsigned char*)frame.data, frame.cols*frame.rows*frame.elemSize(), frame.cols, frame.rows, frame.channels());
//		cvShowManyImages(wName, frame.cols, frame.rows, 5, (unsigned char*)frame.data, (unsigned char*)front.data, (unsigned char*)Y_Cb.data, (unsigned char*)Cr_Cb.data, (unsigned char*)colorMask.data);
		imshow(wName, frame);
		if(fireCount>fireThreshold)
		{
			//count the frame that contains firePixel surpass threshold
			isFire = true;
		}
		else
		{
			isFire = false;
		}

		return fireCount;
}

int FireDetector::getFirePixelNumber(Mat aFrame) {
	std::vector<std::vector<cv::Point> > contours;

	Mat YCrCbFrame;
	Mat YChannel, CrChannel, CbChannel;
	Mat Y_Cb, Cr_Cb;
	Mat colorMask;

	//check for input frame
	if(aFrame.empty())
	{
		return -1;
	}
	//---------------detect moving pixel------------//
	//       using BackgroundSubstractMOG2 			//
	//----------------------------------------------//
	bg.operator ()(aFrame, front);
	bg.getBackgroundImage(back);
	//cv::erode(front,front, cv::Mat());
	//cv::dilate(front, front, cv::Mat());
	cv::medianBlur(front, front, 5);
	cv::findContours(front,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	for(unsigned int i = 0; i < contours.size(); i++ )
	{
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
		minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
	}

	for(unsigned int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( 255,255,255 );
		//params:	  input   output       contourIdx	color    thickness
		drawContours( front, contours_poly,    i,       color,   CV_FILLED, 8, vector<Vec4i>(), 0, Point() );
	}

	//----detect fire color----//
	//-------------------------------------------------------------------//
	//	pixel = fire color when											 //
	//			valueY > valueCb &&										 //
	//			valueCr > valueCb &&									 //
	//			(valueY > meanY && valueCr > meanCr && valueCb < meanCb) //
	//-------------------------------------------------------------------//

	//get YCrCb channel
	cvtColor(aFrame, YCrCbFrame, CV_BGR2YCrCb);
	vector<Mat> channels(3);
	split(YCrCbFrame, channels);
	YChannel = channels[0];
	CrChannel = channels[1];
	CbChannel = channels[2];

	//calculate mean of 3 channels: => for further use
//	unsigned char Y_mean, Cr_mean, Cb_mean;
//	Y_mean = (unsigned char)mean(YChannel)[0];
//	Cr_mean = (unsigned char)mean(CrChannel)[0];
//	Cb_mean = (unsigned char)mean(CbChannel)[0];

	colorMask = Mat(aFrame.rows, aFrame.cols, CV_8UC1);
	Y_Cb  = Mat(aFrame.rows, aFrame.cols, CV_8UC1);//YChannel minus CbChannel
	Cr_Cb = Mat(aFrame.rows, aFrame.cols, CV_8UC1);//CrChannel minus CbChannel
	subtract(YChannel, CbChannel, Y_Cb); threshold(Y_Cb, Y_Cb, 10, 255, THRESH_BINARY);
	subtract(CrChannel, CbChannel, Cr_Cb);threshold(Cr_Cb, Cr_Cb, 10, 255, THRESH_BINARY);

	//colorMask = front & Y_Cb & Y_Cr
	bitwise_and(front, Y_Cb, colorMask);
	bitwise_and(colorMask, Cr_Cb, colorMask);

	int fireCount = countNonZero(colorMask);

	cvtColor(front, front, CV_GRAY2BGR);
	cvtColor(Y_Cb, Y_Cb, CV_GRAY2BGR);
	cvtColor(Cr_Cb, Cr_Cb, CV_GRAY2BGR);
	cvtColor(colorMask, colorMask, CV_GRAY2BGR);

	char wName[25];
	sprintf(&(wName[0]),"Frames_%s", sourceName.c_str());
//	cvShowManyImages(wName, frame.cols, frame.rows, 5, (unsigned char*)frame.data, (unsigned char*)front.data, (unsigned char*)Y_Cb.data, (unsigned char*)Cr_Cb.data, (unsigned char*)colorMask.data);
//	imshow(wName, frame);
	if(fireCount>fireThreshold)
	{
		//count the frame that contains firePixel surpass threshold
		isFire = true;
	}
	else
	{
		isFire = false;
	}

	return fireCount;
}



#if DUAL_THREAD
void* FireDetector::run(void* arg)
{
	FireDetector* obj = (FireDetector*)arg;
	cv::Mat aFrame;
	while(true)
	{
		pthread_mutex_trylock(&obj->runMutex);
		while(frameQueue.empty())
		{
			pthread_cond_wait(&obj->runCond, &obj->runMutex);
		}
		while(!frameQueue.empty())
		{
			std::cout << "Queue not empty" << std::endl;
			unsigned char* data = (unsigned char*)frameQueue.front();
			frameQueue.pop();
			cv::Mat img = cv::Mat( obj->getImgHeight(), obj->getImgWidth(), CV_8UC3, data);
			int start_s=clock();
			int firePixel = obj->getFirePixelNumber(img);//notification was send inside this function
			int stop_s=clock();
			std::cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << std::endl;
			free(data);
		}
		pthread_mutex_unlock(&obj->runMutex);
	}
	return NULL;
}
#else



void* FireDetector::run(void* arg)
{
	FireDetector* obj = (FireDetector*)arg;
	cv::Mat aFrame, tempFrame;
	Mat back;
	Mat front;
	BackgroundSubtractorMOG2 bg;
	unsigned int frameCount = 0;
	char wName[25];
		sprintf(&(wName[0]),"Video_%d", obj->getSourceId());
	while(true)
	{
		if(!obj->capture.read(tempFrame))
		{
			std::cout << "[Run Loop "<< obj->getSourceId()<<"]Cannot read frame" << std::endl;
			obj->capture.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
			continue;
		}
		cv::resize(tempFrame, aFrame, cv::Size(640,480), 0, 0, cv::INTER_CUBIC);
		//pthread_mutex_lock(&obj->runMutex);
//		if(!obj->has_new_image)
//		{
//			obj->has_new_image = true;
//		}
		obj->setFrame(aFrame);
		//pthread_mutex_unlock(&obj->runMutex);

//		int start_s=clock();
		int firePixel = obj->getFirePixelNumber(aFrame);
//		int firePixel = obj->getFirePixelNumber(bg, aFrame, back, front);
		if(firePixel>obj->getFireThreshold())
		{
			frameCount++;
			if(frameCount>30)
			{
				obj->isFire = true;
				frameCount = 0;
			}
		}
//		int stop_s=clock();
//		std::cout << "time(ms): " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << std::endl;
//		imshow(wName, aFrame);
		char c = waitKey(DELAY_TIME_MS);
		if(c=='q')
		{
			std::cout << "Try to quit by button" << std::endl;
		}
	}
	return NULL;
}
#endif


void FireDetector::init()
{
	capture = cv::VideoCapture(sourceVideo);
	if(!capture.isOpened())
	{
		std::cout << "Cannot open video source!" << std::endl;
	}
	std::cout << "[FireDetector_"<< this->getSourceId() << "]Init" << std::endl;
//	display = new DisplayThread(sourceName);
//	display->initDisplayThread();

}

void FireDetector::start()
{
#if DUAL_THREAD
	if( pthread_create(&captureThread,NULL,FireDetector::captureFrame,(void*)this)!=0) //using myCode
	{
		std::cout << "Fail to create captureThread" << std::endl;
	}
#endif

	if( pthread_create(&runThread,NULL,FireDetector::run,(void*)this)!=0) //using myCode
	{
		std::cout << "Fail to create fireThread" << std::endl;
	}
	std::cout << "[FireDetector_"<< this->getSourceId() << "]Start" << std::endl;
//	display->startDisplayThread();
}

void FireDetector::join()
{
	pthread_join(runThread, NULL);
#if DUAL_THREAD
	pthread_join(captureThread, NULL);
#endif
}

void* FireDetector::captureFrame(void* arg)
{
	cv::Mat aFrame;
	FireDetector* obj = (FireDetector*)arg;
	char wName[25];
	sprintf(&(wName[0]),"Video_%d", obj->getSourceId());

	while(true)
	{
		if(!(obj->capture).read(aFrame))
		{
			std::cout << "[Main FireDetector]Cannot read frame" << std::endl;
			obj->capture.set(CV_CAP_PROP_POS_AVI_RATIO, 0);
			//break;
		}
		else
		{
			//std::cout << "[Main FireDetector]Copy to run thread" << std::endl;
			//set image width and height
			int w,h;
			w = aFrame.cols;
			h = aFrame.rows;
			if(w!=obj->getImgWidth())
			{
				obj->setImgWidth(w);
			}
			if(h!=obj->getImgHeight())
			{
				obj->setImgHeight(h);
			}
//			//queue the receive frame
//			pthread_mutex_lock(&obj->runMutex);
//			int data_size = obj->frame.cols*obj->frame.rows*obj->frame.elemSize()*sizeof(char);
//			unsigned char* temp_data = (unsigned char*)malloc(data_size);
//			memcpy(temp_data, obj->frame.data, data_size);
//			frameQueue.push(temp_data);
//			std::cout << "Push to queue" << std::endl;
////			imshow("Frame", obj->frame);
//			pthread_cond_signal(&obj->runCond);
//			pthread_mutex_unlock(&obj->runMutex);

			//imshow(wName, aFrame);
		}
		char c = waitKey(DELAY_TIME_MS);
		if(c=='q')
		{
			break;
		}
	}
	return NULL;
}


