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

ServerThread::ServerThread() {
	// TODO Auto-generated constructor stub
	initServerThread();

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
						buffer[4] = 0x00;
						if(obj->hasFire>=0)
						{
							buffer[4] |= (0x01 << obj->hasFire);
							obj->hasFire = -1;
						}
						if(obj->hasIntruder!=0)
						{
							buffer[4] |= (0x01 << obj->hasIntruder);
							obj->hasIntruder = 0;
						}
//						bool sendOK = sendToSocket.send(buffer, 5);
						sendToSocket.send( (char*)buffer, 5);
						pthread_mutex_unlock(&(obj->serverMutex));

						cout << "Client sent: " ;
						for(int i=0;i<5;i++)
						{
							printf("%2.2x ", (unsigned char)buffer[i]);
						}
						cout << endl;


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
	pthread_mutex_lock(&serverMutex);
	switch(type)
	{
		case 0://Fire
		case 1:
		case 2:
		case 3:
	//	if(!hasFire)
		{
//			std::cout << "Fire Callback to ServerThread's function" << std::endl;
			hasFire = type;
	//		pthread_cond_signal(&serverCond);
		}
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
	pthread_mutex_unlock(&serverMutex);
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
