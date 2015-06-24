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

using namespace cv;
class FireDetector {
public:
	FireDetector();
	virtual ~FireDetector();
	int getFirePixelNumber(Mat input);
private:
	Mat frame;
	Mat back;
	Mat front;
	BackgroundSubtractorMOG2 bg;
	void cvShowManyImages(char* title, int s_cols, int s_rows, int nArgs, ...);
};

#endif /* SRC_FIREDETECTOR_FIREDETECTOR_H_ */
