/*
 * main.cpp
 *
 *  Created on: May 9, 2015
 *      Author: quyen
 */

int main(int argc, void* args[])
{
	//Start MainThread
	MainThread* mainObj = new MainThread();
	mainObj->initMainThread();
	mainObj->startMainThread();
	mainObj->joinMainThread();


	return 0;
}
