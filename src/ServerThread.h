/*
 * ResultServerThread.h
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#ifndef SOURCE_RESULTSERVERTHREAD_H_
#define SOURCE_RESULTSERVERTHREAD_H_
#include <pthread.h>
#include <queue>
class ServerThread {
public:
	ServerThread();
	virtual ~ServerThread();
//private:
	pthread_t 		serverThread;
	pthread_t 		monitorServerThread;
	pthread_mutex_t serverMutex;
	pthread_cond_t 	hasImgCond, noImgCond;

	unsigned char hasFire, hasIntruder;

	std::queue<unsigned char*> videoBufferQueue;



	bool debug_server;



public:
//	bool hasFire;
	void startServerThread();
	void initServerThread();
	void joinServerThread();
	static void handleFireDetected(void* arg, int source);
	static void handleIntruderDetected(void* arg, int source);
	static void handleVideoReady(void* arg, unsigned char* buffer);

	static void* monitor(void* arg);

	void sendAlarm(int type);

	void setDebugPrint(bool debug);
};

#endif /* SOURCE_RESULTSERVERTHREAD_H_ */
