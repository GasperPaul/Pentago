/*
 * CrossThreadMutex.h
 *
 *  Created on: 12 груд. 2013
 *      Author: youw
 */

#ifndef CROSSTHREADMUTEX_H_
#define CROSSTHREADMUTEX_H_

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
