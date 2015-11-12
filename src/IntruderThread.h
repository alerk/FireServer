/*
 * IntruderThread.h
 *
 *  Created on: Jul 6, 2015
 *      Author: nguyen
 */

#ifndef SRC_INTRUDERTHREAD_H_
#define SRC_INTRUDERTHREAD_H_

#include "CommonDefine.h"


typedef void (*CallbackPtr)(void*, int);

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
	void 			connectCallback(CallbackPtr cb, void* cbHandler);
	void 			setDebugPrint(bool debug);
};

#endif /* SRC_INTRUDERTHREAD_H_ */
