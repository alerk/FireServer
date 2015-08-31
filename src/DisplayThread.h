///*
// * DisplayThread.h
// *
// *  Created on: Jul 29, 2015
// *      Author: quyen
// */
//
//#ifndef SRC_DISPLAYTHREAD_H_
//#define SRC_DISPLAYTHREAD_H_
//#include <queue>
//#include <iostream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <pthread.h>
//
//class DisplayThread {
//	typedef void (*CallbackPtr)(void*, int);
//
//public:
//	DisplayThread();
//	DisplayThread(std::string a_name);
//	virtual ~DisplayThread();
//
//private:
//	pthread_t 		displayThread;
//	pthread_mutex_t	displayMutex;
//	pthread_cond_t 	displayCond;
//
//
//
//public:
//	std::string		name;
//	void			enqueue(unsigned char* data, unsigned int length, unsigned int width, unsigned int height, unsigned int depth);
//	void 			startDisplayThread();
//	void 			initDisplayThread();
//	void 			joinDisplayThread();
//	CallbackPtr 	displayDetected;
//	void*			handler;//Actual object that handles the call
//	void 			connectCallback(CallbackPtr cb, void* cbHandler);
//	static void* 	runDisplay(void* arg);
//	void 			cvShowManyImages(std::string title, int s_cols, int s_rows,int nArgs,...);
//};
//
//#endif /* SRC_DISPLAYTHREAD_H_ */
