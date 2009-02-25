/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: singleton.h
 * Description:
        This file is to define and surpport different complier and system.
 * Author: cch
 * Update: 2009-2-21
 * Verison: 0.0.2
=============================================================================*/

#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>

namespace srdgame {

class Mutex
{
	friend class Condition;
public:
	Mutex();
	~Mutex();
	inline void lock()
	{
		pthread_mutex_lock(&_mutex);
	}
	inline void unlock()
	{
		pthread_mutex_unlock(&_mutex);
	}
	inline bool trylock()
	{
		return (pthread_mutex_trylock(&_mutex) == 0);
	}

protected:
	static bool s_attr_initialized;
	static pthread_mutexattr_t s_attr;
	pthread_mutex_t _mutex;
};

}

#endif
