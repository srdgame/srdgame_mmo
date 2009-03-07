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

#ifndef HAVE_DARWIN
#include <semaphore.h>

#include <cassert>

#define ASSERT(x) assert(x)

#include "log.h"

namespace srdgame
{
unsigned int generate_thread_id();

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
		LogDebug("ThreadCtl", "Setup with thread id: %d", thread);
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
unsigned int GenerateThreadId();
class ThreadCtl
{
public:
	ThreadCtl() : _cond(_mutex), _id(0)
	{
	}
	
	~ThreadCtl()
	{
	}

	void bind(pthread_t thread)
	{
		_thread = thread;
		_id = GenerateThreadId();
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
	Condition _cond;
	Mutex _mutex;
	unsigned int _id;
	pthread_t _thread;
};
}
#endif

#endif
