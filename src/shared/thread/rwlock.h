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
#ifndef RW_LOCK_H_
#define RW_LOCK_H_

#include "condition.h"
#include "mutex.h"

namespace srdgame
{
class RWLock
{
public:
	inline RWLock() : _cond(_mutex), _readers(0), _writers(0)
	{
	}
	inline ~RWLock()
	{
	}

	inline void read_lock()
	{
		_cond.lock();
		++_readers;
		_cond.unlock();
	}

	inline void read_unlock()
	{
		_cond.lock();
		if (!--_readers && _writers)
		{
			_cond.signal();
		}
		_cond.unlock();
	}

	inline void write_unlock()
	{
		if (--_writers)
		{
			_cond.signal();
		}
		_cond.unlock();

	}
	
	inline void write_lock()
	{
		_cond.lock();
		++_writers;
		if (_readers)
		{
			_cond.wait();
		}
	}
private:
	Mutex _mutex;
	Condition _cond;
	volatile unsigned int _readers;
	volatile unsigned int _writers;
};
}

#endif
