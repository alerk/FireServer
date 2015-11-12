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

using namespace std;
unsigned char sendBuffer[MSG_BUFFER_SIZE];
unsigned char hasBuffer = 0;

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
//	if( pthread_create(&monitorServerThread,NULL,ServerThread::monitor,(void*)this)!=0)
//		{
//			std::cout << "Fail to create monitorServerThread" << std::endl;
//		}
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
	server_port = iniparser_getint(ini, "server_thread:port",13579);
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
//					cout << "Inside loop!" << endl;
//					pthread_mutex_trylock(&(obj->serverMutex));
//					while(hasBuffer<=0)
//					{
////						std::cout << "Under lock server" << std::endl;
////						pthread_cond_wait(&(obj->hasImgCond), &(obj->serverMutex));
//					}
					if(hasBuffer>0)
					{

//					std::cout << "Prepare to send" << std::endl;
					int sendSize = sendToSocket.send( (char*)sendBuffer, sizeof(sendBuffer));
					hasBuffer--;
//					pthread_mutex_unlock(&(obj->serverMutex));
					if(sendSize<0)
					{
						cout << "Client disconnected" << endl;
						break;
					}
					else
					{
//						cout << "Send to client " << sendSize << " bytes" << endl;
					}
					}
					usleep(1);
				}
				catch(SocketException& e)
				{
					std::cout << "Error while accepting client " << e.description() << std::endl;
					break;
				}
			}
			sleep(2);
		}
	}
	catch(SocketException& e)
	{
		std::cout << "Error while creating server " << e.description() << std::endl;
	}
	return NULL;
}

static void* __run(void* arg)
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

void ServerThread::handleVideoReady(void* arg, unsigned char* buffer)
{
	ServerThread* objServer = (ServerThread*)arg;
//	pthread_mutex_trylock(&(objServer->serverMutex));
//	while(hasBuffer)
//	{
//		pthread_cond_wait(&(objServer->noImgCond),&(objServer->serverMutex));
//	}
//	std::cout << "Has image to send" << std::endl;
	hasBuffer++;
	sendBuffer[0] = 0xff;
	sendBuffer[1] = 0xff;
	sendBuffer[2] = 0xaa;
	sendBuffer[3] = 0x55;
	memcpy(&(sendBuffer[4]), buffer, MSG_BUFFER_SIZE-4);

//	pthread_mutex_unlock(&(objServer->serverMutex));
}

void ServerThread::handleIntruderDetected(void* arg, int source)
{
	ServerThread* objServer = (ServerThread*)arg;
	objServer->sendAlarm(source);

}


void* ServerThread::monitor(void* arg)
{
	ServerThread* obj = (ServerThread*)arg;
	while(true)
	{
//		pthread_mutex_lock(&(obj->serverMutex));
//		if(hasBuffer)
//		{
//			pthread_mutex_lock(&(obj->serverMutex));
////			std::cout << "Unlock condition" << std::endl;
//			pthread_cond_signal(&(obj->hasImgCond));
//			pthread_mutex_unlock(&(obj->serverMutex));
//		}
//		else
//		{
//			pthread_mutex_lock(&(obj->serverMutex));
//			pthread_cond_signal(&(obj->noImgCond));
//			pthread_mutex_unlock(&(obj->serverMutex));
//		}

		usleep(1);
	}
}

void ServerThread::setDebugPrint(bool debug)
{
	debug_server = debug;
}
