/*
 * ResultServerThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

/**
 * Modified on: Oct 5, 2015
 * 		Author: quyen
 * 		Reason:
 * 			- Server only handles the send message
 * 			- The message was prepared by the functioning threads
 *
 */

#ifdef SINGLE_PROCESS
#include "ServerThread.h"
#include "Socket/ServerSocket.h"
#include "Socket/SocketException.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <queue>

#include "ini_parser/iniparser.h"
#include "CommonDefine.h"
#define SERVER_PORT 13579
static void* run(void* arg);


static unsigned char counter = 0;
static int server_port = 0;
static queue<unsigned char*> sendBuffer;

using namespace std;

ServerThread::ServerThread() {
	// TODO Auto-generated constructor stub
//	initServerThread();
	debug_server = false;

}

ServerThread::~ServerThread() {
	// TODO Auto-generated destructor stub
	pthread_cancel(serverThread);
}

void ServerThread::startServerThread() {
	if( pthread_create(&serverThread,NULL,run,(void*)this)!=0)
	{
		std::cout << "Fail to create serverThread" << std::endl;
	}
	std::cout << "[ServerThread]Start" << std::endl;
}

void ServerThread::initServerThread() {
	dictionary* ini;
	std::string src_str;
	ini = iniparser_load("Resources/FireServer.conf");
	if(ini==NULL)
	{
		fprintf(stderr, "[serverThread]cannot parse file\n");
		return;
	}
	server_port = iniparser_getint(ini, "server_thread:port",12345);
	iniparser_freedict(ini);
	std::cout << "[ServerThread]Init" << std::endl;
}

void ServerThread::joinServerThread() {
	pthread_join(serverThread, NULL);
}

static void* run(void* arg)
{
	ServerThread* obj = (ServerThread*)arg;
	try{

		ServerSocket listenServer(server_port);
		while(true)
		{
			ServerSocket sendToSocket;
			cout << "Waiting for client!" << endl;
			listenServer.accept(sendToSocket);
			cout << "Client connected!" << endl;
			while(true)
			{
				try{
					unsigned char buffer[5];
					memset(buffer,0,5);
					buffer[0] = 0xaa;
					buffer[1] = 0x55;
					buffer[2] = 0x00;
					buffer[3] = (counter++);
					//						while(!obj->hasFire)
					//						{
					//							std::cout << "[ServerThread] Run but no signal" << std::endl;
					//							pthread_cond_wait(&(obj->serverCond), &(obj->serverMutex));
					//						}
					pthread_mutex_trylock(&(obj->serverMutex));
					//if(obj->hasFire)
					buffer[4] = 0x0F & obj->hasFire;

					obj->hasFire = 0;

					if(obj->hasIntruder!=0)
					{
						buffer[4] |= (0x01 << obj->hasIntruder);
						obj->hasIntruder = 0;
					}
					//						bool sendOK = sendToSocket.send(buffer, 5);
					bool isSendOk = sendToSocket.send( (char*)buffer, 5);
					pthread_mutex_unlock(&(obj->serverMutex));
					if(!isSendOk)
					{
						cout << "Client disconnected" << endl;
						break;
					}
					else
					{
						cout << "Client sent: " ;
						for(int i=0;i<5;i++)
						{
							printf("%2.2x ", (unsigned char)buffer[i]);
						}
						cout << endl;
					}

				}
				catch(SocketException& e)
				{
					std::cout << "Error while accepting client " << e.description() << std::endl;
					break;
				}
				sleep(2);
			}
		}
	}
	catch(SocketException& e)
	{
		std::cout << "Error while creating server " << e.description() << std::endl;
	}
	return NULL;


}

void ServerThread::sendAlarm(int type) {
	//pthread_mutex_lock(&serverMutex);
	switch(type)
	{
	case 0://Fire
		hasFire |= (0x01<<0);
		break;
	case 1:
		hasFire |= (0x01<<1);
		break;
	case 2:
		hasFire |= (0x01<<2);
		break;
	case 3:
		hasFire |= (0x01<<3);
		break;
	case SRC_INTRUDER://Intruder
	{
		//			std::cout << "Intruder Callback to ServerThread's function" << std::endl;
		hasIntruder = SRC_INTRUDER;
	}
	break;
	default:
		break;
	}
	//pthread_mutex_unlock(&serverMutex);
}

void ServerThread::handleFireDetected(void* arg, int source)
{
	ServerThread* objServer = (ServerThread*)arg;
	objServer->sendAlarm(source);

}

void ServerThread::handleIntruderDetected(void* arg, int source)
{
	ServerThread* objServer = (ServerThread*)arg;
	objServer->sendAlarm(source);

}


#else

#include "ServerThread.h"
#include "Socket/ServerSocket.h"
#include "Socket/SocketException.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <queue>

#include "ini_parser/iniparser.h"
#include "CommonDefine.h"
#include "Util/MessageBuilder.h"


using namespace std;
#define SERVER_PORT 13579

static unsigned char network_buffer[SOCKET_BUFFER_SIZE];
static int server_port = 0;
const unsigned int MAX_BUFFER_SIZE = 300;


ServerThread::ServerThread() {
	// TODO Auto-generated constructor stub
//	initServerThread();
	debug_server = false;
}

ServerThread::~ServerThread() {
	// TODO Auto-generated destructor stub
	pthread_cancel(serverThread);
}

void ServerThread::startServerThread() {
	if( pthread_create(&serverThread,NULL,ServerThread::run,(void*)this)!=0)
	{
		std::cout << "Fail to create serverThread" << std::endl;
	}
	std::cout << "[ServerThread]Start" << std::endl;

	if( pthread_create(&monitorThread,NULL,ServerThread::monitor,(void*)this)!=0)
		{
			std::cout << "Fail to create monitorThread" << std::endl;
		}
		std::cout << "[MonitorThread]Start" << std::endl;
}

void ServerThread::initServerThread() {
	dictionary* ini;
	std::string src_str;
	ini = iniparser_load("Resources/FireServer.conf");
	if(ini==NULL)
	{
		fprintf(stderr, "[serverThread]cannot parse file\n");
		return;
	}
	server_port = iniparser_getint(ini, "server_thread:port",12345);
	iniparser_freedict(ini);
	isLockedRead = isLockedWrite = false;
	std::cout << "[ServerThread]Init" << std::endl;
}

void ServerThread::joinServerThread() {
	pthread_join(serverThread, NULL);
}

void ServerThread::handleSendData(void* arg, unsigned char* buffer)
{
	ServerThread* objServer = (ServerThread*)arg;
	objServer->enqueueMessage(buffer);
}

void* ServerThread::monitor(void* arg)
{
	ServerThread* obj = (ServerThread*)arg;
	while(true)
	{
		obj->checkQueueStatus();
		sleep(1);
	}
	return NULL;
}

void* ServerThread::run(void* arg)
{
	ServerThread* obj = (ServerThread*)arg;
	try
	{
		ServerSocket listenServer(server_port);
		while(true)
		{
			ServerSocket sendToSocket;
			cout << "Waiting for client!" << endl;
			listenServer.accept(sendToSocket);
			cout << "Client connected!" << endl;
			while(true)
			{
				try
				{
					obj->dequeueMessage(&(network_buffer[0]));
					int sentSize = sendToSocket.send( (const char*)(&(network_buffer[0])), SOCKET_BUFFER_SIZE);
					if(sentSize<0)
					{
						cout << "Client disconnected" << endl;
						break;
					}
					else
					{
						cout << "Client sent: "<< sentSize << endl;
					}
				}
				catch(SocketException& e)
				{
					std::cout << "Error while accepting client " << e.description() << std::endl;
				}
			}
		}
	}
	catch(SocketException& e)
	{
		std::cout << "Error while creating server " << e.description() << std::endl;
	}
	return NULL;


}

void ServerThread::enqueueMessage(unsigned char* buffer)
{
	pthread_mutex_lock(&serverMutex);
	while(isBufferFull())
	{
		isLockedWrite = true;
		pthread_cond_wait(&bufferFull, &serverMutex);
	}
	unsigned char* new_buffer = (unsigned char*)malloc(SOCKET_BUFFER_SIZE*sizeof(unsigned char));
	memcpy(new_buffer, buffer, SOCKET_BUFFER_SIZE*sizeof(unsigned char));
	this->messageBuffer.push(new_buffer);
	pthread_mutex_unlock(&serverMutex);
}

void ServerThread::dequeueMessage(unsigned char* buffer)
{
	pthread_mutex_lock(&serverMutex);
	while(!isBufferEmpty())
	{
		isLockedRead = true;
		pthread_cond_wait(&bufferEmpty, &serverMutex);
	}
	unsigned char* temp_buffer = messageBuffer.front();
	memcpy(buffer, temp_buffer, SOCKET_BUFFER_SIZE*sizeof(unsigned char));
	free(temp_buffer);
	messageBuffer.pop();
	pthread_mutex_unlock(&serverMutex);
}

void ServerThread::checkQueueStatus() {
	if(isLockedWrite && !isBufferFull()){
		pthread_cond_signal(&bufferFull);
		isLockedWrite = false;
	}
	if(isLockedRead && !isBufferEmpty()){
		pthread_cond_signal(&bufferEmpty);
		isLockedRead = false;
	}
}

bool ServerThread::isBufferEmpty()
{
	return (messageBuffer.empty());
}

bool ServerThread::isBufferFull()
{
	return (messageBuffer.size() >= MAX_BUFFER_SIZE);
}
#endif
void ServerThread::setDebugPrint(bool debug)
{
	debug_server = debug;
}
