/************************************************************************/
/* File contains some config values that are used on system				*/
/************************************************************************/

#define FRAME_NUMBER_OF_ROWS 240
#define FRAME_NUMBER_OF_COLS 320
/* ==================================================================== */
/* Fire detector constants */
/* ------------------------*/
/* STEP1 */
#define STEPI_LIGHT_PARAM 110
/* STEP2 */
#define CONFIGED_NUMBER_MODE 1
#define INPUT_IMAGES_SAMPLES_MODE 2
/* This is the total number of training images to train the Fire-colored detector system. Training should be fire image with various colors */
#define NUMBER_OF_FIRE_SAMPLES 10	
#define FIRE_FILE1 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/0.png"
#define FIRE_FILE2 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/1.png"
#define FIRE_FILE3 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/2.png"
#define FIRE_FILE4 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/3.png"
#define FIRE_FILE5 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/4.png"
#define FIRE_FILE6 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/5.png"
#define FIRE_FILE7 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/6.png"
#define FIRE_FILE8 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/7.png"
#define FIRE_FILE9 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/8.png"
#define FIRE_FILE10 "/home/quyen/Work/eclipse/workspace/VFDS/imageclipper/9.png"
// another define of fire colored
#define FIRE_COLOR_DIS1_MEAN_RED 121.16
#define FIRE_COLOR_DIS1_MEAN_GREEN 76.87
#define FIRE_COLOR_DIS1_MEAN_BLUE 43.98
#define FIRE_COLOR_DIS1_VARIANCE 10.057

#define FIRE_COLOR_DIS2_MEAN_RED 169.08
#define FIRE_COLOR_DIS2_MEAN_GREEN 84.14
#define FIRE_COLOR_DIS2_MEAN_BLUE 35.20
#define FIRE_COLOR_DIS2_VARIANCE 10.137

#define FIRE_COLOR_DIS3_MEAN_RED 177.00
#define FIRE_COLOR_DIS3_MEAN_GREEN 104.00
#define FIRE_COLOR_DIS3_MEAN_BLUE 62.00
#define FIRE_COLOR_DIS3_VARIANCE 10.00

#define FIRE_COLOR_DIS4_MEAN_RED 230.42
#define FIRE_COLOR_DIS4_MEAN_GREEN 113.78
#define FIRE_COLOR_DIS4_MEAN_BLUE 43.71
#define FIRE_COLOR_DIS4_VARIANCE 10.35

#define FIRE_COLOR_DIS5_MEAN_RED 254.47
#define FIRE_COLOR_DIS5_MEAN_GREEN 214.66
#define FIRE_COLOR_DIS5_MEAN_BLUE 83.08
#define FIRE_COLOR_DIS5_VARIANCE 10.00

#define FIRE_COLOR_DIS6_MEAN_RED 254.97
#define FIRE_COLOR_DIS6_MEAN_GREEN 159.06
#define FIRE_COLOR_DIS6_MEAN_BLUE 151.00
#define FIRE_COLOR_DIS6_VARIANCE 10.00

#define FIRE_COLOR_DIS7_MEAN_RED 254.98
#define FIRE_COLOR_DIS7_MEAN_GREEN 140.98
#define FIRE_COLOR_DIS7_MEAN_BLUE 141.93
#define FIRE_COLOR_DIS7_VARIANCE 10.02

#define FIRE_COLOR_DIS8_MEAN_RED 254.99
#define FIRE_COLOR_DIS8_MEAN_GREEN 146.95
#define FIRE_COLOR_DIS8_MEAN_BLUE 102.99
#define FIRE_COLOR_DIS8_VARIANCE 9.98

#define FIRE_COLOR_DIS9_MEAN_RED 255.00
#define FIRE_COLOR_DIS9_MEAN_GREEN 174.08
#define FIRE_COLOR_DIS9_MEAN_BLUE 175.01
#define FIRE_COLOR_DIS9_VARIANCE 10.05

#define FIRE_COLOR_DIS10_MEAN_RED 255.00
#define FIRE_COLOR_DIS10_MEAN_GREEN 217.96
#define FIRE_COLOR_DIS10_MEAN_BLUE 176.07
#define FIRE_COLOR_DIS10_VARIANCE 10.04

/* ------------------------*/
/* STEP3 */
#define FLICKER_FREQUENCY 20					// this config depends on fps of video camera 
#define FRAME_BUFFER_SIZE FLICKER_FREQUENCY*3
#define THRESHOLD_COUNTER_PASS_ZERO_FILTER_D 8	// threshold to determinate that flicker or not. If zero Pass > threshold -> fire
#define THRESHOLD_COUNTER_PASS_ZERO_FILTER_E 7	// threshold to determinate that flicker or not. If zero Pass > threshold -> fire
#define THRESHOLD_DN_DEC 4	//x10				// if dn > this threshold -> count Dn += 1.
#define THRESHOLD_EN_DEC 4	//x10				// if en > this threshold -> count En += 1.
/* ------------------------*/
/* STEP4 */
#define THRESHOLD_SUBIMAGE_SPATIAL_ENERGY_MIN 0		//predefined MIN threshold to determinate fire-region or not.
#define THRESHOLD_SUBIMAGE_SPATIAL_ENERGY_MAX 1000		//predefined MAX threshold to determinate fire-region or not.
/* ------------------------*/
/* ------------------------*/
#define  THRESHOLD_ALARM_FIRE_REGION_CRITICAL 0.9 // number % of fire pixels on each small region.RED
#define  THRESHOLD_ALARM_FIRE_REGION_MEDIUM 0.6 // number % of fire pixels on each small region.YEALLOW
#define  THRESHOLD_ALARM_FIRE_REGION_CONSIDER 5 // number of fire pixels on each small region.GREEN
/* ==================================================================== */
/* Smoke detector constants */
