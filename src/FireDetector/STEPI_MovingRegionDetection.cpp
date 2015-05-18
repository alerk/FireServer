#include "STEPI_MovingRegionDetection.h"
#include <stdio.h>

//V4 with avgThreshold, the return data type is struct IplImage
void getMovingPixels(IplImage* oneDI_n, IplImage* oneDI_nm1, int lightParam, unsigned char* threshold, IplImage* setOfStationPixel)
{
	//setOfStationPixel 1: moving, 0: station
	
	/************************************************************************/
	/*                       RUN ON PRE-DEFINED HW PLATFORM                 */
	/************************************************************************/
	#ifdef  USING_HW_KERNEL
		//run with hware kernel
		printf( "old version!\n" );
	#endif

	/************************************************************************/
	/*                       RUN ON SOFTWARE PLATFORM                       */
	/************************************************************************/
	#ifdef  USING_SW_OPT
		int frameHeigh;
		int frameWidth;
		int absvalue = 0;
		float total = 0;
		int i,j;

		frameHeigh = oneDI_n->height;
		frameWidth = oneDI_n->width;

		for (i = 0; i < frameHeigh; i++)
		{
			unsigned char* ptr_Inm1 = (unsigned char*)(oneDI_nm1->imageData + i*frameWidth);
			unsigned char* ptr_In = (unsigned char*)(oneDI_n->imageData + i*frameWidth);
			unsigned char* ptr_setOfStationPixel = (unsigned char*)(setOfStationPixel->imageData + i*frameWidth);			
			for ( j = 0; j < frameWidth; j++)
			{
				absvalue = abs(ptr_In[j] - ptr_Inm1[j]);
				if ( absvalue > *threshold)
				{
					ptr_setOfStationPixel[j] = 255;				
				}
				else
				{
					ptr_setOfStationPixel[j] = 0;
				}
				//re-assign
				total += absvalue;				
			}			
		}
		total = lightParam*total / (frameWidth*frameHeigh);
		*threshold = total;		
	#endif
}

