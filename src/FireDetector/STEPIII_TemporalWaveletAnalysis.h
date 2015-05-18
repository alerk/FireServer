#ifndef STEPIII_TEMPORAL_WAVELET_ANALYSIS
#define STEPIII_TEMPORAL_WAVELET_ANALYSIS

#include "CommonFunctions.h"

void getFlickerPixel(IplImage *frame[FRAME_BUFFER_SIZE],int nrow_image,int ncol_image,IplImage* MaskOfIdxFirePoints, IplImage* setOfIdxFlickerPoints);
//unsigned char* getFlickerPixel(IplImage *frame[FRAME_BUFFER_SIZE],int nrow_image,int ncol_image);

#endif
