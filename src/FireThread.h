/*
 * FireDetectorThread.h
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#ifndef SOURCE_FIREDETECTORTHREAD_H_
#define SOURCE_FIREDETECTORTHREAD_H_
#include <pthread.h>
#include <tr1/functional>

typedef void (*CallbackPtr)(void*);

class FireThread {
public:
	FireThread();
	virtual ~FireThread();

private:
	pthread_t 		fireThread;
	pthread_mutex_t fireMutex;
	pthread_cond_t 	fireCond;
public:
	void 			startFireThread();
	void 			initFireThread();
	void 			joinFireThread();
	CallbackPtr 	fireDetected;
	void*			handler;//Actual object that handles the call
	void 			connectCallback(CallbackPtr cb, void* cbHandler);
//	std::tr1::function <void (void)> fireDetected;//callback pointer
	//static void (FireThread::*fireDetected)(void);//quyen callback
};

#endif /* SOURCE_FIREDETECTORTHREAD_H_ */
