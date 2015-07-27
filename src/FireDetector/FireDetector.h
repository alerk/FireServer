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


using namespace cv;
class FireDetector {
public:
	FireDetector(int id, std::string name, std::string input, int threshold);
	virtual ~FireDetector();
	int getFirePixelNumber(Mat input);

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

	bool hasFire;
	VideoCapture capture;

	pthread_t 		captureThread;
	pthread_t 		runThread;
	pthread_mutex_t runMutex;
	pthread_cond_t 	runCond;

private:
	int sourceId;
	std::string sourceName;
	std::string sourceVideo;
	int fireThreshold;

	//timer
	struct itimerval it_val;

	Mat frame;
	Mat back;
	Mat front;
	BackgroundSubtractorMOG2 bg;
	int imgWidth, imgHeight;

	void cvShowManyImages(char* title, int s_cols, int s_rows, int nArgs, ...);
	void init();
	void start();
	void join();

	static void* captureFrame(void* arg);
	static void* run(void* arg);



};

#endif /* SRC_FIREDETECTOR_FIREDETECTOR_H_ */
