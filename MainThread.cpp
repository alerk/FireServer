/*
 * MainThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#include "MainThread.h"

#include <unistd.h>
static void* run(void* arg);
MainThread::MainThread() {
	// TODO Auto-generated constructor stub

}

MainThread::~MainThread() {
	// TODO Auto-generated destructor stub
}

void MainThread::startMainThread() {
	fireObj->startFireThread();
	serverObj->startServerThread();
	if( pthread_create(&mainThread,NULL,run,(void*)this)!=0)
		{
			std::cout << "Fail to create serverThread" << std::endl;
		}
}

void MainThread::initMainThread() {
	fireObj = new FireThread();
	fireObj->initFireThread();
	serverObj = new ServerThread();
	serverObj->initServerThread();
}

void MainThread::joinMainThread() {
	fireObj->joinFireThread();
	serverObj->joinServerThread();
	pthread_join(mainThread, NULL);
}

static void* run(void* arg)
{
	MainThread* mainObj = (MainThread*)arg;
	while(true)
	{

	}
	return NULL;
}
