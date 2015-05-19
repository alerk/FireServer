/*
 * FireDetectorThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#include "FireThread.h"
#include <iostream>
#include <unistd.h>
static void* run(void* arg);
FireThread::FireThread() {
	// TODO Auto-generated constructor stub

}

FireThread::~FireThread() {
	// TODO Auto-generated destructor stub
}

static void* run(void* arg)
{
	FireThread* mainObj = (FireThread*)arg;
	while(true)
	{
		sleep(1);

	}
	return NULL;
}

void FireThread::startFireThread() {
	if( pthread_create(&fireThread,NULL,run,(void*)this)!=0)
	{
		std::cout << "Fail to create fireThread" << std::endl;
	}
}

void FireThread::initFireThread() {
}

void FireThread::joinFireThread() {
	pthread_join(fireThread, NULL);
}
