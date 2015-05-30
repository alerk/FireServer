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

public:
	void startFireThread();
	void initFireThread();
	void joinFireThread();
	void (*fireDetected)(void);
};

#endif /* SOURCE_FIREDETECTORTHREAD_H_ */
