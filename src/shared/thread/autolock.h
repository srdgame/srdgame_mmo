/**============================================================================
 * Copyright (C) 2009 SrdGame Team
 *
 * File: singleton.h
 * Description:
        This file is to define and surpport different complier and system.
 * Author: cch
 * Update: 2009-2-21
 * Verison: 0.0.2
 * Message: I copy this from my orginal project:RioWow, and adjust the coding
 * style
=============================================================================*/

#ifndef AUTOLOCK_H_
#define AUTOLOCK_H_

class AutoLock
{
public:
	AutoLock(Mutex& mutex) : _mutex(mutex)
	{
		_mutex.lock();
	}
	~AutoLock()
	{
		_mutex.unlock();
	}
	AutoLock& operator= (AutoLock& obj)
	{
		this->_mutex = obj._mutex;
		return *this;
	}
private:
	Mutex& _mutex;
}

#endif
