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
#include <sys/time.h>       /* for setitimer */
#include <unistd.h>     /* for pause */
#include <signal.h>     /* for signal */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "../DisplayThread.h"


using namespace cv;
class FireDetector {
public:
	FireDetector(int id, std::string name, std::string input, int threshold);
	virtual ~FireDetector();
	int getFirePixelNumber(Mat input);
	int getFirePixelNumber(BackgroundSubtractorMOG2 bg, Mat frame, Mat back, Mat front);

	int getSourceId() const {
		return sourceId;
	}

	int getImgHeight() const {
		return imgHeight;
	}

	int getImgWidth() const {
		return imgWidth;
	}

	void setImgHeight(int imgHeight) {
		this->imgHeight = imgHeight;
	}

	void setImgWidth(int imgWidth) {
		this->imgWidth = imgWidth;
	}

	int getFireThreshold() const {
		return fireThreshold;
	}

	void setSourceId(int sourceId) {
		this->sourceId = sourceId;
	}

	bool isFire;
	VideoCapture capture;

	pthread_t 		captureThread;
	pthread_t 		runThread;
	pthread_mutex_t runMutex;
	pthread_cond_t 	runCond;

	void init();
	void start();
	void join();

	int getFrame(unsigned char* buff)  {
		if(!isHasNewImage())
		{
			return 0;
		}
		else
		{
			pthread_mutex_lock(&runMutex);
			unsigned int imgSize = frame.cols*frame.rows*frame.elemSize();
			setHasNewImage(false);
			memcpy(buff, frame.data, imgSize);
			//std::cout << "[FireDetector "<< this->sourceId << "]Return frame!" << std::endl;
			pthread_mutex_unlock(&runMutex);
			return 1;
		}
	}

	void setFrame(const Mat& frame) {
		pthread_mutex_lock(&runMutex);
		//std::cout << "[FireDetector "<< this->sourceId << "]Set frame!" << std::endl;
		setHasNewImage(true);
		this->imgWidth = frame.cols;
		this->imgHeight = frame.rows;
		this->frame = frame.clone();
//		pthread_cond_signal(&runCond);
		pthread_mutex_unlock(&runMutex);
	}

	bool isHasNewImage() const {
		return has_new_image;
	}

	void setHasNewImage(bool hasNewImage) {
		has_new_image = hasNewImage;
	}

private:
	int sourceId;
	std::string sourceName;
	std::string sourceVideo;
	int fireThreshold;
	bool has_new_image;
	//DisplayThread* display;

	//timer
	struct itimerval it_val;

	Mat frame;
	Mat back;
	Mat front;
	BackgroundSubtractorMOG2 bg;
	int imgWidth, imgHeight;

	void cvShowManyImages(std::string title, int s_cols, int s_rows, int nArgs, ...);



	static void* captureFrame(void* arg);
	static void* run(void* arg);



};

#endif /* SRC_FIREDETECTOR_FIREDETECTOR_H_ */
