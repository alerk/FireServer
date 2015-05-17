#ifndef STEPIV_SPATIAL_WAVELET_ANALYSIS
#define STEPIV_SPATIAL_WAVELET_ANALYSIS

#include "CommonFunctions.h"
/* Function helps to take LH,HL,HH values of sub-images*/
/* return a region is fire or just a fire-colored object*/
/* input image should be the Y-channel */
int isFireRegion(IplImage * originalImgae);

/* Check subRegion contain pixel fire_colored is fire region*/
int isFireRegionV2(IplImage * Y_channel_image, int TopLeft_X,int TopLeft_Y,int rect_w,int rect_h);

/* function do the step 4 */
void STEPIV_SpactionVariance(IplImage* gray_currentframe,IplImage* setOfFlickerPoints, IplImage* MaskOfIdxFirePoints);

#endif
