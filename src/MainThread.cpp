/*
 * MainThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#include "MainThread.h"
#include <iostream>
#include <unistd.h>
#include <tr1/functional>

static void* run(void* arg);
MainThread::MainThread() {
	// TODO Auto-generated constructor stub
	initMainThread();

}

MainThread::~MainThread() {
	// TODO Auto-generated destructor stub
	delete(fireObj);
	delete(serverObj);
	delete(intruderObj);
	pthread_cancel(mainThread);
}

void MainThread::startMainThread() {
	if( pthread_create(&mainThread,NULL,run,(void*)this)!=0)
	{
		std::cout << "Fail to create serverThread" << std::endl;
	}
	fireObj->startFireThread();
	serverObj->startServerThread();
}

void MainThread::initMainThread() {
	fireObj = new FireThread();
	//fireObj->initFireThread();
	serverObj = new ServerThread();
	//serverObj->initServerThread();
	intruderObj = new IntruderThread();

	//assign callback from fireObj to serverObj
	fireObj->connectCallback(ServerThread::handleFireDetected, serverObj);
	intruderObj->connectCallback(ServerThread::handleIntruderDetected, serverObj);

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

		sleep(1);

	}
	return NULL;
}
