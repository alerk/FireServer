/*
 * FireDetectorThread.h
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#ifndef SOURCE_FIREDETECTORTHREAD_H_
#define SOURCE_FIREDETECTORTHREAD_H_
#include <pthread.h>
#define MAX_NUMBER_OF_INPUT 10

typedef void (*CallbackPtr)(void*, int);

class FireThread {
public:
	FireThread();
	virtual ~FireThread();

private:
	pthread_t 		fireThread;
	pthread_mutex_t fireMutex;
	pthread_cond_t 	fireCond;

	bool result[MAX_NUMBER_OF_INPUT];


public:
	void 			startFireThread();
	void 			initFireThread();
	void 			joinFireThread();
	CallbackPtr 	fireDetected;
	void*			handler;//Actual object that handles the call
	void 			connectCallback(CallbackPtr cb, void* cbHandler);
	static void* 	runFireThread(void* arg);
};

#endif /* SOURCE_FIREDETECTORTHREAD_H_ */
