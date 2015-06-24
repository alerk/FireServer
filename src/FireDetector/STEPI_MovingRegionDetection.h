#ifndef STEPI_MOVING_REGION_DETECTION
#define STEPI_MOVING_REGION_DETECTION

#include "CommonFunctions.h"

void getMovingPixels(IplImage* oneDI_n, IplImage* oneDI_nm1, int lightParam, unsigned char* threshold,IplImage* setOfStationPixel);
//void getMovingPixels_(cv::Mat input, cv::Mat& movingMap);

#endif //STEPI_MOVING_REGION_DETECTION
