/*
 * FireDetector.h
 *
 *  Created on: Jun 24, 2015
 *      Author: nguyen
 */

#ifndef SRC_FIREDETECTOR_FIREDETECTOR_H_
#define SRC_FIREDETECTOR_FIREDETECTOR_H_
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>


#include <pthread.h>
#include <sys/time.h>       /* for set itimer */
#include <unistd.h>     /* for pause */
#include <iostream>

#include "../DisplayThread.h"

typedef void (*CallbackPtr)(void*, unsigned char*);

using namespace cv;
class FireDetector {
public:
	FireDetector(int id, std::string name, std::string input, int threshold);
	virtual ~FireDetector();
	int getFirePixelNumber(Mat input);
	int getFirePixelNumber(BackgroundSubtractorMOG2 bg, Mat frame, Mat back, Mat front);


	int getSourceId() const;
	int getImgHeight() const;
	int getImgWidth() const;
	void setImgHeight(int imgHeight);
	void setImgWidth(int imgWidth);
	int getFireThreshold() const ;
	void setSourceId(int sourceId);
	void init();
	void start();
	void join();

	int getFrame(unsigned char* buff);

	void setFrame(const Mat& frame);
	void connectCallback(CallbackPtr cb, void* cbHandler);

	bool isHasNewImage() const;
	void setHasNewImage(bool hasNewImage);
	void setBuffer(unsigned char* buff);

	bool isFire;
	VideoCapture capture;

	pthread_t 		captureThread;
	pthread_t 		runThread;
	pthread_mutex_t runMutex;
	pthread_cond_t 	runCond;

	//callback function variables
	CallbackPtr 	videoCaptured;
	void*			handler;
private:
	int sourceId;
	std::string sourceName;
	std::string sourceVideo;
	int fireThreshold;
	bool has_new_image;
	//timer
	struct itimerval it_val;
	//opencv
	Mat frame;
	Mat back;
	Mat front;
	BackgroundSubtractorMOG2 bg;
	int imgWidth, imgHeight;
	unsigned char* imgBuff;

	//void cvShowManyImages(std::string title, int s_cols, int s_rows, int nArgs, ...);
	static void* run(void* arg);
};

#endif /* SRC_FIREDETECTOR_FIREDETECTOR_H_ */
