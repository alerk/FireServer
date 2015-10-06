/*
 * IntruderThread.h
 *
 *  Created on: Jul 6, 2015
 *      Author: nguyen
 */

#ifndef SRC_INTRUDERTHREAD_H_
#define SRC_INTRUDERTHREAD_H_

#include "CommonDefine.h"

#ifdef SINGLE_PROCESS
typedef void (*CallbackPtr)(void*, int);
#else
typedef void (*CallbackPtr)(void*, unsigned char*);
#endif
class IntruderThread {
public:
	IntruderThread();
	virtual ~IntruderThread();


	bool debug_intruder;

public:
	void 			startIntruderThread();
	void 			initIntruderThread();
	void 			joinIntruderThread();
	CallbackPtr 	intruderDetected;
	void*			handler;//Actual object that handles the call
	unsigned char msg_buffer[SOCKET_BUFFER_SIZE];
	void 			connectCallback(CallbackPtr cb, void* cbHandler);
	void 			setDebugPrint(bool debug);
};

#endif /* SRC_INTRUDERTHREAD_H_ */
