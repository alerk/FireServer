#pragma once

#include <opencv2/imgproc/imgproc_c.h>  // Gaussian Blur
#include <opencv2/core/core_c.h>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui_c.h>  // OpenCV window I/O
#include <math.h>
#include "define.h"
#include "Config.h"

float getABS(float a);
IplImage * RGB_Y_convert(IplImage* RGBImage);
void getMotionAreas(int *x_cor, int *y_cor, unsigned char* idxAreaPixes);
