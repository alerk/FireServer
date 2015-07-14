/*
 * IntruderThread.h
 *
 *  Created on: Jul 6, 2015
 *      Author: nguyen
 */

#ifndef SRC_INTRUDERTHREAD_H_
#define SRC_INTRUDERTHREAD_H_

typedef void (*CallbackPtr)(void*, int);

class IntruderThread {
public:
	IntruderThread();
	virtual ~IntruderThread();

public:
	void 			startIntruderThread();
	void 			initIntruderThread();
	void 			joinIntruderThread();
	CallbackPtr 	intruderDetected;
	void*			handler;//Actual object that handles the call
	void 			connectCallback(CallbackPtr cb, void* cbHandler);
};

#endif /* SRC_INTRUDERTHREAD_H_ */
