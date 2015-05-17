#include "STEPIII_TemporalWaveletAnalysis.h"


void getFlickerPixel(IplImage *frame[FRAME_BUFFER_SIZE],int nrow_image,int ncol_image,IplImage* MaskOfIdxFirePoints, IplImage* setOfIdxFlickerPoints)
{
	int x,y,index_dn_cn,i,j;
	int index_en;
	int count_threshold_dn;
	int count_threshold_en;
	float en_avg;
	float abs_temp;
	unsigned char* ptrTemp, *ptr;
	float dn_avg;
	float dn[FRAME_BUFFER_SIZE/2];
	float cn[FRAME_BUFFER_SIZE/2];
	float en[FRAME_BUFFER_SIZE/4];
	float temp_dn[FRAME_BUFFER_SIZE];
	float temp_cn[FRAME_BUFFER_SIZE];
	float temp_en[FRAME_BUFFER_SIZE/2];
	unsigned char Xn[FRAME_BUFFER_SIZE];
	//setOfIdxFlickerPoints;//0	1666666  : fire-colored object. otw: 1

	// calculate output of filter-bank 1
	for ( x = 0; x < nrow_image; x++)
	{
		unsigned char* ptr_MaskOfIdxFirePoints = (unsigned char*)(MaskOfIdxFirePoints->imageData + x*ncol_image);
		ptrTemp = (unsigned char*)(setOfIdxFlickerPoints->imageData + x*ncol_image);
		for ( y = 0; y < ncol_image; y++)
		{
			if (ptr_MaskOfIdxFirePoints[y] == 255)
			{
				/************************************************************************/
				/*     [HW VERISON] CALL TO SOFTWARE FUNCTION BELOWED THIS COMMENT      */
				/************************************************************************/
				/*for ( i = 0 ; i< FRAME_BUFFER_SIZE ;i+=1)
				{
					Xn[i] = *((unsigned char*)(frame[i]->imageData	+ x*frame[i]->widthStep		+ y));
				}
				ptrTemp = ((unsigned char*)(setOfIdxFlickerPoints + x*ncol_image*sizeof(unsigned char) + y));
				executeSTEP3_8bits_version(Xn,
					THRESHOLD_DN_DEC,
					THRESHOLD_EN_DEC,
					THRESHOLD_COUNTER_PASS_ZERO_FILTER_D,
					THRESHOLD_COUNTER_PASS_ZERO_FILTER_E,
					ptrTemp);*/
				/************************************************************************/
				/*     [HW VERISON] CALL TO SOFTWARE FUNCTION ABOVED THIS COMMENT       */
				/************************************************************************/
			
				//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

				/************************************************************************/
				/*     [SW VERISON] CALL TO SOFTWARE FUNCTION BELOWED THIS COMMENT      */
				/************************************************************************/
				//for each pixel
				//1. calculate dn

				for (i = 0; i < FRAME_BUFFER_SIZE; i++)
				{
					ptr = (unsigned char*)(frame[i]->imageData	+ x*frame[i]->widthStep + y);
					Xn[i] = *ptr;
				}

				for ( i = 0 ; i< FRAME_BUFFER_SIZE-2 ;i=i+1)
				{
					temp_dn[i] = -0.25*Xn[i] + 0.5*Xn[i+1] - 0.25*Xn[i+2]; //HPF
					temp_cn[i] =  0.25*Xn[i] - 0.5*Xn[i+1] + 0.25*Xn[i+2]; //LPF
				}
				temp_dn[FRAME_BUFFER_SIZE-2] = 0;
				temp_dn[FRAME_BUFFER_SIZE-1] = 0;
				temp_cn[FRAME_BUFFER_SIZE-2] = 0;
				temp_cn[FRAME_BUFFER_SIZE-1] = 0;

				//down-sample 2
				for ( i = 0 ; i< FRAME_BUFFER_SIZE/2 ;i+=1)
				{
					dn[i] = temp_dn[2*i]; //down-sample 2
					cn[i] = temp_cn[2*i]; //down-sample 2
				}

				//2. calculate output of filter-bank 2, en
				for ( i = 0 ; i< (FRAME_BUFFER_SIZE/2) - 2 ;i=i+1)
				{
					temp_en[i] = -0.25*cn[i] + 0.5*cn[i+1] - 0.25*cn[i+2]; //HPF
				}
				temp_en[(FRAME_BUFFER_SIZE/2) - 2] = 0;
				temp_en[(FRAME_BUFFER_SIZE/2) - 1] = 0;

				//down-sample 2
				for ( i = 0 ; i< FRAME_BUFFER_SIZE/4 ;i+=1)
				{
					en[i] = temp_en[2*i]; //down-sample 2
				}

				//3. calculate threshold changing.
				count_threshold_dn = 0;
				count_threshold_en = 0;
				for ( i = 0; i<FRAME_BUFFER_SIZE/2; i++)
				{
					//printf("[dn]%d\n",(int)(dn[i]));
					abs_temp = abs(dn[i]);
					if (abs_temp>THRESHOLD_DN_DEC)
					{
						count_threshold_dn += 1;
					}

				}
				for ( i = 0; i<FRAME_BUFFER_SIZE/4 - 1; i++)
				{
					abs_temp = abs(en[i]);
					if (abs_temp>THRESHOLD_EN_DEC)
					{
						count_threshold_en += 1;
					}

				}

				//4. estimate and classify if it is fire pixel moving
				//ptrTemp = ((unsigned char*)(setOfIdxFlickerPoints->imageData + x*ncol_image + y));
				if (count_threshold_dn > THRESHOLD_COUNTER_PASS_ZERO_FILTER_D &&
					count_threshold_en > THRESHOLD_COUNTER_PASS_ZERO_FILTER_E)
				{
					ptrTemp[y] = 255;
				}else{
					ptrTemp[y] = 0;
				}
				/************************************************************************/
				/*         [SW VERISON] CALL TO SOFTWARE FUNCTION ABOVED THIS COMMENT   */
				/************************************************************************/
			}else{
				ptrTemp[y] = 0;
			}
			
		}
	}
	
}