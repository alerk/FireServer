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


using namespace cv;
class FireDetector {
public:
	FireDetector();
	FireDetector(int id, std::string name, std::string input, int threshold);
	virtual ~FireDetector();
	int getFirePixelNumber(Mat input);
	int getFirePixelNumber(std::string input);

	int getSourceId() const {
		return sourceId;
	}

	bool hasFire;
	VideoCapture capture;

private:
	int sourceId;
	std::string sourceName;
	std::string sourceVideo;
	int fireThreshold;


	Mat frame;
	Mat back;
	Mat front;
	BackgroundSubtractorMOG2 bg;

	void cvShowManyImages(char* title, int s_cols, int s_rows, int nArgs, ...);
	void init();
	void start();
	void join();

	pthread_t 		runThread;
	pthread_mutex_t runMutex;
	pthread_cond_t 	runCond;

};

#endif /* SRC_FIREDETECTOR_FIREDETECTOR_H_ */
