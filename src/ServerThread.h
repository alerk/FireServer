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
	pthread_t 		serverThread;
	pthread_mutex_t serverMutex;
	pthread_cond_t 	serverCond;

	unsigned char hasFire, hasIntruder;
	bool debug_server;


private:
	static std::queue<unsigned char*> messageBuffer;



public:
//	bool hasFire;
	void startServerThread();
	void initServerThread();
	void joinServerThread();
	static void handleSendData(void* arg, int source, unsigned char* data);

#if 0
	static void handleFireDetected(void* arg, int source);
	static void handleIntruderDetected(void* arg, int source);
#endif
	void pushMessage(unsigned char* buffer);
	void popMessage(unsigned char* buffer);
	bool hasMessage();
	void sendAlarm(int type);
	void setDebugPrint(bool debug);
	static void* run(void* args);
};

#endif /* SOURCE_RESULTSERVERTHREAD_H_ */
