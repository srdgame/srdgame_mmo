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

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "singleton.h"
#include "mutex.h"
#include "threadctl.h"
#include "threadbase.h"
#include <set>

namespace srdgame
{

class ThreadPool : public Singleton<ThreadPool>
{
protected:
	struct ThreadHandle
	{
		ThreadCtl controller;
		ThreadBase* task; // WHen you create such obect, please set this one to NULL if there is no task to run. or CRASH!!!
		bool reuse; // Please use true to keep this thread for all coming task, unless you are sure you need't this thread anymore.
		Mutex lock; // Please lock this before change anything of this struct, except controller?
	};
public:
	ThreadPool();
	~ThreadPool();

	// Init the thread pool with capacity.
	void init(unsigned int cap = 10);
	// Shutdown the thread pool and all the threads..
	void shutdown();
	
	// Execute one task
	void execute(ThreadBase* task);

	// For debug
	void print_state();

protected:
	// Used to adjust the pool
	class PoolAdjust : public ThreadBase
	{
		virtual bool run();
		virtual void on_close();
		virtual void shutdown();
		virtual bool is_running();
	};

	// Adjust pool, expecially the threads which are free.
	void adjust();

	// When one thread want to run its task, return true will delete the thread or hold it for next run
	bool on_thread_run(ThreadHandle* thread);

	// When one thread finishes his job.
	bool on_thread_finish(ThreadHandle* thread);

	// Create one new thread to execut task.
	ThreadHandle* create_thread(ThreadBase* task);

	// call this to remove one thread that will not be used later.
	void remove_thread(ThreadHandle* thread);

	// Free idle threads
	void kill_idle_thread(unsigned int count);

	// static function
	static void* thread_proc(void* param);

protected:
	typedef std::set<ThreadHandle*> ThreadSet;

	// The set contains all the active threads information.
	ThreadSet _active_set;

	// The set contains all the idle threads' information.
	ThreadSet _idle_set;

	// Pool has been initialized or not.
	bool _inited;

	// Lock for pool attributes.
	Mutex _lock;
};
}

#endif
