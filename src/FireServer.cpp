//============================================================================
// Name        : FireServer.cpp
// Author      : QuyenNX
// Version     :
// Copyright   : ITT Solution
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "MainThread.h"
using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	MainThread* mainObj = new MainThread();
	mainObj->initMainThread();
	mainObj->startMainThread();
	mainObj->joinMainThread();
	return 0;
}
