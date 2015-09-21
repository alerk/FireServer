/*
 * MainThread.h
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#ifndef SOURCE_MAINTHREAD_H_
#define SOURCE_MAINTHREAD_H_
#include <pthread.h>
#include "FireThread.h"
#include "ServerThread.h"
#include "IntruderThread.h"

class MainThread {
public:
	MainThread();
	virtual ~MainThread();

private:
	pthread_t 		mainThread;
	pthread_mutex_t mainMutex;
	pthread_cond_t 	mainCond;

	FireThread* fireObj;
	ServerThread* serverObj;
	IntruderThread* intruderObj;

	bool debug_print;

public:
	void startMainThread();
	void initMainThread();
	void joinMainThread();


};

#endif /* SOURCE_MAINTHREAD_H_ */
