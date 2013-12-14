/*
 * CrossThreadMutex.h
 *
 *  Created on: 12 груд. 2013
 *      Author: youw
 */

#ifndef CROSSTHREADMUTEX_H_
#define CROSSTHREADMUTEX_H_

#ifdef _WIN32
#include <windows.h>
#elif __linux__//_WIN32
#include <unistd.h>
#define Sleep(x) usleep(x*1000)
#endif //_WIN32

//Like std::mutex but can be unlocked from
//thread that did not lock it before
class CrossThreadMutex {
public:
	CrossThreadMutex(bool _isLocked = false);
	~CrossThreadMutex();
	void wait();
	void lock();
	bool try_lock();
	void unlock();
	bool is_locked();
private:
	bool isLocked;
};

#endif /* CROSSTHREADMUTEX_H_ */
