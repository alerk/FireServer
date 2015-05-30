/*
 * FireDetectorThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#include "FireThread.h"
#include <iostream>
#include <unistd.h>

#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"

static void* run(void* arg);
static CvCapture capture;
FireThread::FireThread()
{
	// TODO Auto-generated constructor stub

}

FireThread::~FireThread()
{
	// TODO Auto-generated destructor stub
}

static void* run(void* arg)
{
	FireThread* mainObj = (FireThread*)arg;
	while(true)
	{

		mainObj->fireDetected();

		sleep(1);

	}
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
	capture = cvCaptureFromFile("../Resources/outdoor_daytime_10m_heptane_CCD_001.avi");

}

void FireThread::joinFireThread()
{
	pthread_join(fireThread, NULL);
}
