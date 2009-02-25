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
	ThreadBase()
	{
	}
	virtual ~ThreadBase()
	{
	}
	virtual bool run() = 0;
	virtual bool on_close() = 0;
}
}

#endif
