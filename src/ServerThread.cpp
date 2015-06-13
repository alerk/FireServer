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
#define SERVER_PORT 13579
static void* run(void* arg);

static unsigned char counter = 0;
static int serverPort = 0;
using namespace std;

ServerThread::ServerThread() {
	// TODO Auto-generated constructor stub

}

ServerThread::~ServerThread() {
	// TODO Auto-generated destructor stub
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
	serverPort = iniparser_getint(ini, "server_thread:port",12345);
	iniparser_freedict(ini);
}

void ServerThread::joinServerThread() {
	pthread_join(serverThread, NULL);
}

static void* run(void* arg)
{
	ServerThread* obj = (ServerThread*)arg;
	try{

		ServerSocket listenServer(serverPort);
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
						if(obj->hasFire)
						{
							buffer[4] = 0x01;
							obj->hasFire = false;
						}
						else
						{
							buffer[4] = 0x00;
						}
//						bool sendOK = sendToSocket.send(buffer, 5);
						sendToSocket.send( (char*)buffer, 5);

						cout << "Client sent: " ;
						for(int i=0;i<5;i++)
						{
							printf("%2.2x ", buffer[i]);
						}
						cout << endl;


					}
					catch(SocketException& e)
					{
						std::cout << "Error while accepting client " << e.description() << std::endl;
						break;
					}
					sleep(1);
				}
			}
	}
	catch(SocketException& e)
	{
		std::cout << "Error while creating server " << e.description() << std::endl;
	}
	return NULL;


}

void ServerThread::sendAlarm() {
	pthread_mutex_lock(&serverMutex);
//	std::cout << "Callback to ServerThread's function" << std::endl;
	hasFire = true;
	pthread_mutex_unlock(&serverMutex);
//	pthread_cond_signal(&serverCond);
}

void ServerThread::handleFireDetected(void* arg)
{
	ServerThread* objServer = (ServerThread*)arg;
	objServer->sendAlarm();

}
