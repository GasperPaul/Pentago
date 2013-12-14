/*
 * CrossThreadMutex.cpp
 *
 *  Created on: 12 груд. 2013
 *      Author: youw
 */

#include "CrossThreadMutex.h"
#ifdef _WIN32
#include <windows.h>
#elif __linux__//_WIN32
#include <unistd.h>
#define Sleep(x) usleep(x*1000)
#endif //_WIN32



CrossThreadMutex::CrossThreadMutex(bool _isLocked) {
	isLocked = _isLocked;
}

CrossThreadMutex::~CrossThreadMutex() {
	// TODO Auto-generated destructor stub
}


void CrossThreadMutex::wait() {
	while (isLocked) {
		//TODO: make it cross platform and fine later
		Sleep(100);
	}
}
void CrossThreadMutex::lock() {
	wait();
	isLocked = true;
}
bool CrossThreadMutex::try_lock() {
	if (!isLocked) {
		return isLocked = true;
	}
	return false;
}
void CrossThreadMutex::unlock() {
	isLocked = false;
}
bool CrossThreadMutex::is_locked() {
	return isLocked;
}
