/*
 * ResultServerThread.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#include "ServerThread.h"
#include "Socket/ServerSocket.h"
#include "Socket/SocketException.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

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
					pthread_mutex_lock(&(obj->serverMutex));
					while(hasBuffer==0)
					{
						pthread_cond_wait(&(obj->serverCond), &(obj->serverMutex));
					}
					bool isSendOk = sendToSocket.send( (char*)sendBuffer, sizeof(sendBuffer));
					hasBuffer = 0;
					pthread_mutex_unlock(&(obj->serverMutex));
					if(!isSendOk)
					{
						cout << "Client disconnected" << endl;
						break;
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
	pthread_mutex_lock(&(objServer->serverMutex));
	hasBuffer = 1;
	sendBuffer[0] = 0xff;
	sendBuffer[1] = 0xff;
	sendBuffer[2] = 0xaa;
	sendBuffer[3] = 0x55;
	sendBuffer[4] = 0x02;
	sendBuffer[5] = 5;
	sendBuffer[6] = 32;
	sendBuffer[7] = 24;
	sendBuffer[8] = 3;
	sendBuffer[9] = 1;
	sendBuffer[10] = 1;
	memcpy(&(sendBuffer[11]), buffer, 4*321*241*3);
	pthread_cond_signal(&(objServer->serverCond));
	//build message here
	pthread_mutex_unlock(&(objServer->serverMutex));
}

void ServerThread::handleIntruderDetected(void* arg, int source)
{
	ServerThread* objServer = (ServerThread*)arg;
	objServer->sendAlarm(source);

}

void ServerThread::setDebugPrint(bool debug)
{
	debug_server = debug;
}
