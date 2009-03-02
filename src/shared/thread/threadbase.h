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

#ifndef THREAD_BASE_H_
#define THREAD_BASE_H_

namespace srdgame
{
class ThreadBase
{
public:
	ThreadBase()
	{
	}
	virtual ~ThreadBase()
	{
	}
	// blocking function, return true means delete this threadobject.
	virtual bool run() = 0;
	// call to shutdown this thread.
	virtual void shutdown() = 0;
	// call when this get closing? not work yet?
	virtual void on_close() = 0;
	// call to checking whether it is running.
	virtual bool is_running() = 0;
};
}

#endif
