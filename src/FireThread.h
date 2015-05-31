/*
 * FireDetectorThread.h
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

#ifndef SOURCE_FIREDETECTORTHREAD_H_
#define SOURCE_FIREDETECTORTHREAD_H_
#include <pthread.h>

class FireThread {
public:
	FireThread();
	virtual ~FireThread();

private:
	pthread_t 		fireThread;
	pthread_mutex_t fireMutex;
	pthread_cond_t 	fireCond;
	typedef void (*callback)(void);

public:
	void startFireThread();
	void initFireThread();
	void joinFireThread();
	static void (FireThread::*fireDetected)(void);//quyen callback
};

#endif /* SOURCE_FIREDETECTORTHREAD_H_ */
