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

#ifndef THREAD_CONTROLLER_H_
#define THREAD_CONTROLLER_H_

#include <pthread.h>

#include <cassert>

#define ASSERT(x) assert(x)

#include "log.h"

#define HAVE_DARWIN

#ifndef HAVE_DARWIN
#include <semaphore.h>


namespace srdgame
{

class ThreadCtl
{
	volatile static unsigned int _s_thread_id_count;
	unsigned int generate_thread_id()
	{
		return ++_s_thread_id_count;
	}

public:
	void setup(pthread_t thread)
	{
		//LogDebug("ThreadCtl", "Setup with thread id: %d", thread);
		_thread = thread;
		sem_init(&_sem, PTHREAD_PROCESS_PRIVATE, 0);
		_id = generate_thread_id();
	}
	~ThreadCtl()
	{
		sem_destroy(&_sem);
	}

	void suspend()
	{
		ASSERT(pthread_equal(pthread_self(), _thread));
		sem_wait(&_sem);
	}

	void resume()
	{
		ASSERT(!pthread_equal(pthread_self(), _thread));
		sem_post(&_sem);
	}

	void join()
	{
		// waits until the thread finishes then returns
		pthread_join(_thread, NULL);
	}

	inline unsigned int get_id() { return _id; }

private:
	sem_t _sem;
	pthread_t _thread;
	unsigned int _id;
};
}
#else

#include "condition.h"

namespace srdgame
{
/*
class ThreadCtl
{
	unsigned int generate_thread_id()
	{
		return ++_s_thread_id_count;
	}

	volatile static unsigned int _s_thread_id_count;
public:
	ThreadCtl() : _id(0)
	{
	}
	
	~ThreadCtl()
	{
		pthread_mutex_destroy(&_mutex);
		pthread_cond_destroy(&_cond);
	}

	void setup(pthread_t thread)
	{
		LogDebug("ThreadCtl", "Setup with thread id: %d", thread);
		_thread = thread;
		pthread_mutex_init(&_mutex, NULL);
		pthread_cond_init(&_cond, NULL);
		_id = generate_thread_id();
	}

	void suspend()
	{
		pthread_cond_wait(&_cond, &_mutex);
	}
	void resume()
	{
		pthread_cond_signal(&_cond);
	}
	void join()
	{
		pthread_join(_thread, NULL);
	}
	unsigned int get_id() { return _id; }

private:
	pthread_cond_t _cond;
	pthread_mutex_t _mutex;
	unsigned int _id;
	pthread_t _thread;
};*/

class ThreadCtl
{
	unsigned int generate_thread_id()
	{
		return ++_s_thread_id_count;
	}

	volatile static unsigned int _s_thread_id_count;
public:
	ThreadCtl() : _cond(_mutex), _id(0)
	{
	}
	
	~ThreadCtl()
	{
	}

	void setup(pthread_t thread)
	{
		LogDebug("ThreadCtl", "Setup with thread id: %d", thread);
		_thread = thread;
		_id = generate_thread_id();
	}

	void suspend()
	{
		_cond.wait();
	}
	void resume()
	{
		_cond.signal();
	}
	void join()
	{
		pthread_join(_thread, NULL);
	}
	unsigned int get_id() { return _id; }

private:
	Mutex _mutex;
	Condition _cond;
	unsigned int _id;
	pthread_t _thread;
};
}
#endif

#endif
