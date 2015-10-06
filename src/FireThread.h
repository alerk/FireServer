/*
 * FireDetectorThread.h
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#ifndef SOURCE_FIREDETECTORTHREAD_H_
#define SOURCE_FIREDETECTORTHREAD_H_
#include <iostream>
#include <pthread.h>
#include "CommonDefine.h"

#define MAX_NUMBER_OF_INPUT 10

#ifdef SINGLE_PROCESS
typedef void (*CallbackPtr)(void*, int);
#else
typedef void (*CallbackPtr)(void*, unsigned char*);
#endif
class FireThread {
public:
	FireThread();
	virtual ~FireThread();
	bool 			debug_fire;

private:
	pthread_t 		fireThread;
	pthread_t 		displayThread;
	pthread_mutex_t fireMutex;
	pthread_cond_t 	fireCond;

public:
	CallbackPtr 	fireDetected;
	void*			handler;//Actual object that handles the call
	unsigned char msg_buffer[SOCKET_BUFFER_SIZE];

	void 			startFireThread();
	void 			initFireThread();
	void 			joinFireThread();

	void 			connectCallback(CallbackPtr cb, void* cbHandler);
	static void* 	runFireThread(void* arg);
	static void* 	runDisplayThread(void* arg);
	void 			setDebugPrint(bool debug);
	void 			cvShowManyImages(std::string title, int s_cols, int s_rows, int nArgs,...);

};

#endif /* SOURCE_FIREDETECTORTHREAD_H_ */
