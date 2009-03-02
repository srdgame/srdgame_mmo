/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: condition.h
 * Description:
        This file is to define the condition class.
 * Author: cch
 * Update: 2009-2-21
 * Verison: 0.0.2
=============================================================================*/

#ifndef CONDITION_H_
#define CONDITION_H_

#include "mutex.h"

namespace srdgame
{
class Condition
{
public:
	inline Condition(Mutex& mutex) : _mutex(mutex)
	{
		//this->_mutex = mutex;
		pthread_cond_init(&_cond, NULL);
	}
	inline ~Condition()
	{
		pthread_cond_destroy(&_cond);
	}
	inline void signal()
	{
		pthread_cond_signal(&_cond);
	}
	inline void broadcast()
	{
		pthread_cond_broadcast(&_cond);
	}
	inline bool wait()
	{
		pthread_cond_wait(&_cond, &_mutex._mutex);
		return true;
	}
	inline bool wait(time_t sec)
	{
		timespec time;
		time.tv_nsec = 0;
		time.tv_sec = sec;
		return (pthread_cond_timedwait(&_cond, &_mutex._mutex, &time) == 0);
	}
	inline void lock()
	{
		_mutex.lock();
	}
	inline void unlock()
	{
		_mutex.unlock();
	}
private:
	pthread_cond_t _cond;
	Mutex& _mutex;
};
}

#endif
