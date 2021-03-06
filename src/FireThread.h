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


typedef void (*CallbackPtr)(void*, int);

typedef void (*CallbackVideo)(void*, unsigned char*);


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
	void 			startFireThread();
	void 			initFireThread();
	void 			joinFireThread();

	CallbackPtr 	fireDetected;
	CallbackVideo	videoReady;
	void*			handler;//Actual object that handles the call

	void 			connectCallback(CallbackPtr cb, void* cbHandler);
	void 			connectCallbackVideo(CallbackVideo cb, void* cbHandler);
	static void* 	runFireThread(void* arg);
	static void* 	runDisplayThread(void* arg);

	void 			setDebugPrint(bool debug);

	void 			cvShowManyImages(std::string title, int s_cols, int s_rows, int nArgs,...);

	void 			combineImages(int s_cols, int s_rows, int nArgs,...);


};

#endif /* SOURCE_FIREDETECTORTHREAD_H_ */
