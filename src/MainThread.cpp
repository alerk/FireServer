/*
 * MainThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#include "MainThread.h"
#include <iostream>
#include <unistd.h>

#include "ini_parser/iniparser.h"

static void* run(void* arg);
MainThread::MainThread() {
	// TODO Auto-generated constructor stub
//	initMainThread();

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
	intruderObj->startIntruderThread();
}

void MainThread::initMainThread() {
	dictionary* ini;
	std::string src_str;
	ini = iniparser_load("Resources/FireServer.conf");
	if(ini==NULL)
	{
		fprintf(stderr, "[MainThread]cannot parse file\n");
		return;
	}
	debug_print = iniparser_getboolean(ini, "main_thread:debug_print",0);
	iniparser_freedict(ini);


	fireObj = new FireThread();
	fireObj->initFireThread();
	fireObj->setDebugPrint(debug_print);

	serverObj = new ServerThread();
	serverObj->initServerThread();
	serverObj->setDebugPrint(debug_print);

	intruderObj = new IntruderThread();
	intruderObj->initIntruderThread();
	intruderObj->setDebugPrint(debug_print);

	//assign callback from fireObj to serverObj
	fireObj->connectCallback(ServerThread::handleFireDetected, serverObj);
	fireObj->connectCallbackVideo(ServerThread::handleVideoReady, serverObj);
	intruderObj->connectCallback(ServerThread::handleIntruderDetected, serverObj);

	std::cout << "[Main Thread]Init" << std::endl;

}

void MainThread::joinMainThread() {
	fireObj->joinFireThread();
	serverObj->joinServerThread();
	intruderObj->joinIntruderThread();
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
