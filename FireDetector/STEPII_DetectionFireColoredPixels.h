#ifndef STEPII_DETECTION_FIRE_COLORED_PIXEL
#define STEPII_DETECTION_FIRE_COLORED_PIXEL

#include "CommonFunctions.h"

typedef struct GaussOfFireSamples_{
	// center of Gauss
	float R_center;
	float G_center;
	float B_center;
	// Radius
	double R_radius;
	double G_radius;
	double B_radius;
} GaussOfFireSamples;

// choose 10 fire samples to build Gauss Model.
// Model will be used to check color is fire or not
void createMixtuaGaussModelOfFireSamples(int mode, char * fireName1,char * fireName2,char * fireName3,char * fireName4,char * fireName5,char * fireName6,char * fireName7,char * fireName8,char * fireName9,char * fireName10);
int isFireColored(unsigned char R_value,unsigned char G_value,unsigned char B_value);
//void setOfColorPixel(unsigned char* movingPixels,unsigned char* MaskOfIdxFirePoints, IplImage* RGBcurrentframe, int grayWidthStep);
void setOfColorPixel(IplImage* movingPixels,IplImage* MaskOfIdxFirePoints, IplImage* RGBcurrentframe);

#endif //STEPII_DETECTION_FIRE_COLORED_PIXEL
