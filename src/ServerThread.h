/*
 * ResultServerThread.h
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#ifndef SOURCE_RESULTSERVERTHREAD_H_
#define SOURCE_RESULTSERVERTHREAD_H_
#include <pthread.h>
#include <iostream>
#include <queue>

class ServerThread {
public:
	ServerThread();
	virtual ~ServerThread();
//private:
	pthread_t 			serverThread;
	pthread_mutex_t 	serverMutex;
	pthread_cond_t 	bufferEmpty;
	pthread_cond_t 	bufferFull;

	unsigned char hasFire, hasIntruder;
	bool debug_server;


private:
	static std::queue<unsigned char*> messageBuffer;



public:
//	bool hasFire;
	void startServerThread();
	void initServerThread();
	void joinServerThread();

#ifdef SINGLE_PROCESS
	static void handleFireDetected(void* arg, int source);
	static void handleIntruderDetected(void* arg, int source);
#else
	static void handleSendData(void* arg, unsigned char* data);
#endif
	void enqueueMessage(unsigned char* buffer);
	void dequeueMessage(unsigned char* buffer);
	bool hasMessage();
	bool isBufferFull();
	void sendAlarm(int type);
	void setDebugPrint(bool debug);
	static void* run(void* args);
};

#endif /* SOURCE_RESULTSERVERTHREAD_H_ */
