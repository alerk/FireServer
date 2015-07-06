/*
 * IntruderThread.cpp
 *
 *  Created on: Jul 6, 2015
 *      Author: nguyen
 */

#include "IntruderThread.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "Socket/ClientSocket.h"

#define DEFAULT_IP "192.168.100.103"
#define DEFAULT_PORT 112233

using namespace std;
static void* run(void* arg);

	pthread_t 		intruderThread;
	pthread_mutex_t intruderMutex;
	pthread_cond_t 	intruderCond;

	static std::string src_str;
	static int src_port;


IntruderThread::IntruderThread()
{
	// TODO Auto-generated constructor stub
	initIntruderThread();

}

IntruderThread::~IntruderThread()
{
	// TODO Auto-generated destructor stub
	pthread_cancel(fireThread);
}

void IntruderThread::startIntruderThread()
{
	if( pthread_create(&intruderThread,NULL,run,(void*)this)!=0) //using myCode
		{
			std::cout << "Fail to create intruderThread" << std::endl;
		}
}

void IntruderThread::initIntruderThread()
{
	//init the video source here - prepare for multiple inputs
			dictionary* ini;
//			std::string src_str;
//			int src_port;
			ini = iniparser_load("Resources/FireServer.conf");
			if(ini==NULL)
			{
				fprintf(stderr, "cannot parse file\n");
				return;
			}

			src_str = iniparser_getstring(ini,"intruder_detector:source", DEFAULT_IP);
			src_port = iniparser_getint(ini, "intruder_detector:port",DEFAULT_PORT);
			iniparser_freedict(ini);
}

void IntruderThread::joinIntruderThread()
{
	pthread_join(intruderThread, NULL);
}

void IntruderThread::connectCallback(CallbackPtr cb, void* cbHandler)
{
	intruderDetected= cb;
	handler = cbHandler;
}

static void* run(void* arg)
{
	IntruderThread* obj = (IntruderThread*)arg;
	ClientSocket client;
	char recv_buf[5];
	while(true)
	{
		try
		{
			//connect to the socket to get intruder data; this obj is client
			client = ClientSocket(src_str, src_port);
			//if yes, then notify the ServerObj
			while(true)
			{
				int recv_size = client.recv(&(recv_buf[0]), 5);
				if(recv_size<=0)
				{
					throw SocketException ( "Could not read from socket." );
				}
//				printf("Receiving %d bytes from server:", recv_size);
//				for(int i=0;i<5;i++)
//				{
//					printf("%2.2x",(char)recv_buf[i]);
//				}
				if(recv_buf[4]==0x01)
				{
					cout << "\t ====> Intruder!" << endl;
					(obj->intruderDetected)(obj->handler);
				}
//				printf("\n");
			}
		}
		catch(SocketException &ex)
		{
			cout << "Error " << ex.description() << endl;
			sleep(1);
		}

	}
	return NULL;
}
