/*
 * DisplayThread.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: quyen
 */

#include "DisplayThread.h"
#include <stdarg.h>

//#define GAP 10
//#define SIZE_1 360
//#define SIZE_2 320
//#define SIZE_3 280

const int GAP = 10;
const int SIZE_1 = 360;
const int SIZE_2 = 320;
const int SIZE_3 = 280;


typedef struct
{
	unsigned char* data;
	unsigned int length;
	unsigned int w, h, d;
}ImageData;

std::queue<ImageData> 		displayQueue;
static int instant_counter = 0;

DisplayThread::DisplayThread()
{
	// TODO Auto-generated constructor stub

}

DisplayThread::DisplayThread(std::string a_name) {
	this->name = a_name;
	std::cout << "Create display " << a_name << std::endl;
	instant_counter++;
	std::cout << "[DIsplayThread]Instant counter: "<<instant_counter<<std::endl;
//	initDisplayThread();
}

DisplayThread::~DisplayThread()
{
	// TODO Auto-generated destructor stub
}

void DisplayThread::enqueue(unsigned char* data, unsigned int length, unsigned int width, unsigned int height, unsigned int depth)
{
	ImageData temp;
	temp.data = (unsigned char*)malloc(length*sizeof(unsigned char));
	memcpy(temp.data, data, length*sizeof(unsigned char));
	temp.length = length;
	temp.w = width;
	temp.h = height;
	temp.d = depth;
	pthread_mutex_lock(&displayMutex);
	displayQueue.push(temp);
	pthread_cond_signal(&displayCond);
	pthread_mutex_unlock(&displayMutex);
}

void DisplayThread::startDisplayThread()
{
	if( pthread_create(&displayThread,NULL, DisplayThread::runDisplay,(void*)this)!=0) //using myCode
		{
			std::cout << "Fail to create displayThread" << std::endl;
		}
}

void DisplayThread::initDisplayThread()
{
	cv::namedWindow(name);
}

void DisplayThread::joinDisplayThread()
{
	pthread_join(displayThread, NULL);
}

void DisplayThread::connectCallback(CallbackPtr cb, void* cbHandler)
{
}

void* DisplayThread::runDisplay(void* arg)
{
	DisplayThread* obj = (DisplayThread*)arg;

	while(true)
	{
		while(displayQueue.empty())
		{
			pthread_cond_wait(&obj->displayCond, &obj->displayMutex);
		}
		pthread_mutex_trylock(&obj->displayMutex);
		while(!displayQueue.empty())
		{
			ImageData temp = displayQueue.front();
			displayQueue.pop();
			cv::Mat img(temp.h, temp.w, CV_8UC3, temp.data);
			imshow(obj->name, img);
			free(temp.data);
		}
		int c = cv::waitKey(10);
		pthread_mutex_unlock(&obj->displayMutex);
	}

}

void DisplayThread::cvShowManyImages(std::string title, int s_cols, int s_rows,
		int nArgs,...)
{
	// img - Used for getting the arguments
	cv::Mat img;

	// [[disp_image]] - the image in which input images are to be copied
	cv::Mat disp_image;

	int size;
	int i;
	int m, n;
	int x, y;

	// w - Maximum number of images in a row
	// h - Maximum number of images in a column
	int w, h;

	// scale - How much we have to resize the image
	float scale;
	int max;

	// If the number of arguments is lesser than 0 or greater than 12
	// return without displaying
	if(nArgs <= 0)
	{
		std::cout << "Number of arguments too small...." << std::endl;
		return;
	}
	else if(nArgs > 12)
	{
		std::cout << "Number of arguments too large...." << std::endl;
		return;
	}
	// Determine the size of the image,
	// and the number of rows/cols
	// from number of arguments
	else if (nArgs == 1)
	{
		w = h = 1;
		size = SIZE_1;
	}
	else if (nArgs == 2)
	{
		w = 2; h = 1;
		size = SIZE_1;
	}
	else if (nArgs == 3 || nArgs == 4)
	{
		w = 2; h = 2;
		size = SIZE_1;
	}
	else if (nArgs == 5 || nArgs == 6)
	{
		w = 3; h = 2;
		size = SIZE_2;
	}
	else if (nArgs == 7 || nArgs == 8)
	{
		w = 4; h = 2;
		size = SIZE_2;
	}
	else
	{
		w = 4; h = 3;
		size = SIZE_3;
	}

	// Create a new 3 channel image
	disp_image = cv::Mat(cv::Size((w+1)*GAP+size*w, (h+1)*GAP+size*h), CV_8UC3, 3);

	// Used to get the arguments passed
	va_list args;
	va_start(args, nArgs);

	// Loop for nArgs number of arguments
	for (i = 0, m = GAP, n = GAP; i < nArgs; i++, m += (GAP + size))
	{
		// Get the Pointer to the IplImage
		unsigned char* temp = va_arg(args, unsigned char*);

		// Check whether it is NULL or not
		// If it is NULL, release the image, and return
		img = cv::Mat( s_rows, s_cols, CV_8UC3, temp);
		if(img.empty())
		{
			std::cout << "Invalid arguments" << std::endl;
			return;
		}

		// Find the width and height of the image
		x = img.cols;
		y = img.rows;

		// Find whether height or width is greater in order to resize the image
		max = (x > y)? x: y;

		// Find the scaling factor to resize the image
		scale = (float) ( (float) max / size );

		// Used to Align the images
		if( i % w == 0 && m!= GAP)
		{
			m = GAP;
			n+= (GAP + size);
		}

		// Set the image ROI to display the current image
		cv::Rect roi(m, n, (int)(x/scale), (int)( y/scale ));
		cv::Mat image_roi = disp_image(roi);

		// Resize the input image and copy the it to the Single Big Image
		cv::resize(img, img, cv::Size((int)(x/scale), (int)( y/scale )));
		img.copyTo(image_roi);

		// Reset the ROI in order to display the next image
	}

	// Create a new window, and show the Single Big Image
	imshow(title, disp_image);

	// End the number of arguments
	va_end(args);
}
