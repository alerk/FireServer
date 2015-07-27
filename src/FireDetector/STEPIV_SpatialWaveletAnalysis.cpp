#include "STEPIV_SpatialWaveletAnalysis.h"

//check region of 9x9
#define REGION_ROWS 9
#define REGION_COLS 9
int isFireRegion(IplImage * originalImgae) /* OriginalImage should be single Channel */
{
	/************************************************************************/
	/*            [HW VERSION] CALL TO HARDWARE KERNEL                      */
	/************************************************************************/
	/*unsigned char* Xn = (unsigned char*)(originalImgae->imageData);
	unsigned int temp_fireRegion;
	executeSTEP4_8bits_version(Xn,
		THRESHOLD_SUBIMAGE_SPATIAL_ENERGY_MIN,
		THRESHOLD_SUBIMAGE_SPATIAL_ENERGY_MAX,
		&temp_fireRegion);
	return temp_fireRegion;*/
	/************************************************************************/
	/*            [HW VERSION] END                                          */
	/************************************************************************/


	/************************************************************************/
	/*            [sW VERSION] CALL TO HARDWARE KERNEL                      */
	/************************************************************************/
	//CommonFunctions instanceCommon;
	float total_energy_of_subImages = 0.0;
	float low_subImage[REGION_ROWS][REGION_COLS/2];
	float high_subImage[REGION_ROWS][REGION_COLS/2];
	float temp_low_subImage[REGION_ROWS][REGION_COLS];
	float temp_high_subImage[REGION_ROWS][REGION_COLS];

	float low_high_subImage[REGION_ROWS/2][REGION_COLS/2];
	float high_low_subImage[REGION_ROWS/2][REGION_COLS/2];
	float high_high_subImage[REGION_ROWS/2][REGION_COLS/2];
	float temp_low_high_subImage[REGION_ROWS][REGION_COLS/2];
	float temp_high_low_subImage[REGION_ROWS][REGION_COLS/2];
	float temp_high_high_subImage[REGION_ROWS][REGION_COLS/2];

	unsigned char rowData[REGION_ROWS];
	int return_value = 0;

	//int nrow = originalImgae->height;
	//int ncol = originalImgae->width;
	int r,c,i;
	int row_index,col_index;
	if (originalImgae->imageData == '\0')
	{
		return 0;
	}

	//1. Calculate the [1D] spatial on rows
	for (r = 0; r < REGION_ROWS; r++)
	{
		unsigned char* ptrOriginalImgae = (unsigned char*)(originalImgae->imageData + r*originalImgae->widthStep);
		for (i = 0; i < REGION_COLS; i++)
		{
			//copy all data of row r
			rowData[i] = ptrOriginalImgae[i];
		}
		// calculate H and L wavelets on row r
		for ( i = 0 ; i< REGION_COLS-2 ;i++)
		{
			temp_high_subImage[r][i] = -0.25*rowData[i] + 0.5*rowData[i+1] - 0.25*rowData[i+2]; //HPF
			temp_low_subImage[r][i] =  0.25*rowData[i] - 0.5*rowData[i+1] + 0.25*rowData[i+2]; //LPF
		}
		temp_high_subImage[r][REGION_COLS-2] = 0;
		temp_high_subImage[r][REGION_COLS-1] = 0;
		temp_low_subImage[r][REGION_COLS-2] = 0;
		temp_low_subImage[r][REGION_COLS-1] = 0;

		//down-sample 2
		for ( i = 0 ; i< REGION_COLS/2 ;i+=1)
		{
			low_subImage[r][i] = temp_low_subImage[r][2*i]; //down-sample 2
			high_subImage[r][i] = temp_high_subImage[r][2*i]; //down-sample 2
		}
	}


	//2. Calculate the [2D] spatial on cols of H-L subImages above;
	for( c = 0; c< (REGION_COLS/2); c++)
	{
		for (i = 0; i < REGION_ROWS; i++)
		{
			temp_high_low_subImage[i][c] = 0.25*high_subImage[i][c]	+	0.5*high_subImage[i+1][c]	+	0.25*high_subImage[i+2][c];
			temp_low_high_subImage[i][c] = -0.25*low_subImage[i][c]	+	0.5*low_subImage[i+1][c]	-	0.25*low_subImage[i+2][c];
			temp_high_high_subImage[i][c] = -0.25*high_subImage[i][c]+	0.5*high_subImage[i+1][c]	-	0.25*high_subImage[i+2][c];
		}
		temp_high_low_subImage[REGION_ROWS-2][c] = 0;
		temp_high_low_subImage[REGION_ROWS-1][c] = 0;
		temp_low_high_subImage[REGION_ROWS-2][c] = 0;
		temp_low_high_subImage[REGION_ROWS-1][c] = 0;
		temp_high_high_subImage[REGION_ROWS-2][c] = 0;
		temp_high_high_subImage[REGION_ROWS-1][c] = 0;
		//down-sample 2
		for ( i = 0 ; i< REGION_ROWS/2 ;i+=1)
		{
			high_low_subImage[i][c] = temp_high_low_subImage[2*i][c];
			low_high_subImage[i][c] = temp_low_high_subImage[2*i][c];
			high_high_subImage[i][c] = temp_high_high_subImage[2*i][c];
		}
	}


	//3. compare total energy of sub-images with predefined threshold to determinate fire-region or not.
	for ( r = 0; r < REGION_ROWS/2; r++)
	{
		for ( c = 0; c < REGION_COLS/2; c++)
		{
			total_energy_of_subImages += high_low_subImage[r][c]*high_low_subImage[r][c];
			total_energy_of_subImages += low_high_subImage[r][c]*low_high_subImage[r][c];
			total_energy_of_subImages += high_high_subImage[r][c]*high_high_subImage[r][c];
		}
	}
	total_energy_of_subImages /= (float)(REGION_ROWS*REGION_COLS);

	if (total_energy_of_subImages > THRESHOLD_SUBIMAGE_SPATIAL_ENERGY_MIN
		&& total_energy_of_subImages < THRESHOLD_SUBIMAGE_SPATIAL_ENERGY_MAX)
	{
		return_value = 1;
	}
	return return_value;
	/************************************************************************/
	/*            [SW VERSION] END                                          */
	/************************************************************************/
}

int isFireRegionV2(IplImage * Y_channel_image, int TopLeft_X,int TopLeft_Y,int rect_w,int rect_h)
{
	IplImage* ROI_Image;
	CvRect myrect;
	int return_value = 0;
	myrect = cvRect(TopLeft_X,TopLeft_Y,rect_w,rect_h);
	cvSetImageROI(Y_channel_image,myrect);
	ROI_Image = cvCloneImage(Y_channel_image);
	cvResetImageROI(Y_channel_image);

	if (isFireRegion(ROI_Image) == 1)
	{		
		return_value = 1;
	}

	cvReleaseImage(&ROI_Image);
	return return_value;
}

void STEPIV_SpactionVariance(IplImage* Y_frame,IplImage* setOfFlickerPoints, IplImage* MaskOfIdxFirePoints)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int t = 0;
	float row_avg = 0;
	float col_avg = 0;
	int frameHeigh = setOfFlickerPoints->height;
	int frameWidth = setOfFlickerPoints->width;
	
	// for every flicker points, check the windows sized 9x9 to determinate color variations or not
	// if fire area => significantly color variations
	for ( i = 4;i<frameHeigh-4;i = i+1)
	{
		unsigned char* ptr_setOfFlickerPoints = (unsigned char*)(setOfFlickerPoints->imageData + i*frameWidth);
		unsigned char* ptr_MaskOfIdxFirePoints = (unsigned char*)(MaskOfIdxFirePoints->imageData + i*frameWidth);
		for( j = 4; j<frameWidth-4; j = j+1)
		{
			if (ptr_setOfFlickerPoints[j] == 255)
			{
				//get ROI (9x9) of flicker pixel at the center
				if (isFireRegionV2(Y_frame,j-4,i-4,REGION_ROWS,REGION_COLS)==1)
				{
					//area is fire region
					ptr_MaskOfIdxFirePoints[j] = 255;
				}
			}else{
				ptr_MaskOfIdxFirePoints[j] = 0;
			}
		}
	}
}
