#include "threadpool.h"
#include "autolock.h"
#include "log.h"

using namespace srdgame;

#ifdef THREAD_POOL_DEBUG
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

ThreadPool::ThreadPool() : _inited(false)
{
}
ThreadPool::~ThreadPool()
{
	// Shutdown the pool here.
	shutdown();
}
void ThreadPool::init(unsigned int cap)
{
	_LogDebug_("ThreadPool", "Init the pool as capicity: %d", cap);

	// Lock the attributes.
	AutoLock lock(_lock);
	if (_inited)
		return;

	if (cap > 500)
		LogWarning("ThreadPool", "You are creating really too many threads!!!");

	// Create as many as cap is.
	ThreadHandle* handle = NULL;
	while (cap != 0)
	{
		_LogDebug_("ThreadPool", "Create one more thread");
		handle = create_thread(NULL);
		_active_set.insert(handle);
		--cap;
	}

	// set initilized to be true.
	_inited = true;
	LogNotice("ThreadPool", "Initialized the pool with %d threads", _active_set.size());
}

void ThreadPool::shutdown()
{
	LogNotice("ThreadPool", "Shutdown all the threads");
	_lock.lock();
	if (!_inited)
	{
		// Not initialized yet, just quit.
		_lock.unlock();
		return;
	}
	
	// send close message to thread.
	_LogDebug_("ThreadPool", "Sending closing signal to ative threads: %d", _active_set.size());
	for (ThreadSet::iterator ptr = _active_set.begin(); ptr != _active_set.end(); ++ptr)
	{
		// Change the reuse to false, and call the shutdown the thread.
		ThreadHandle* handle = *ptr;
		handle->lock.lock();
		handle->reuse = false;
		handle->lock.unlock();

		// If there is task, just call shutdown.
		if (handle->task)
		{
			handle->task->shutdown();
		}
	}
	// set the reuse to false, and active the thread, then it will be deleted.
	this->kill_idle_thread((unsigned int)_idle_set.size());

	_lock.unlock();

	// Make sure all the threads could existed before pool has been deleted.
	while (true)
	{
		_lock.lock();
		if (_active_set.empty() && _idle_set.empty())
		{
			_inited = false;
			_lock.unlock();
			break;
		}
		_lock.unlock();
		//Sleep(1000);
	}
	LogSuccess("ThreadPool", "Completed shutdown all the threads");
}

void ThreadPool::execute(ThreadBase* task)
{
	_LogDebug_("ThreadPool", "Execute task....");
//	LogDebug("", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
	
	_lock.lock();
	
	if (!_inited)
	{
		LogError("ThreadPool", "Could not use the pool before you have initialized it");
		_lock.unlock();
		return;
	}

	ThreadHandle* handle = NULL;

	// If there is any idle threads.
	if (!_idle_set.empty())
	{
		// get one of them, here is the first one.
		handle = *_idle_set.begin();
		// erase from idle set.
		_idle_set.erase(_idle_set.begin());

		// Do we really need to lock it?  let's do it first until there is performance issue.
		// TODO:
		handle->lock.lock();
		handle->task = task;
		handle->lock.unlock();

		// Call resume to let the thread run...
		_LogDebug_("ThreadPool", "Reuse thread handle: %d", handle);
		handle->controller.resume();
	}
	else
	{
		_LogDebug_("ThreadPool", "No more free thread, creating...");
		// Create one more thread for task.
		handle = create_thread(task);
	}

	// Insert the reused or newly created thread to the active set.
	_active_set.insert(handle);

	// unlock the attributes.
	_lock.unlock();
	_LogDebug_("ThreadPool", "Task has been put it run queue");
}

void ThreadPool::print_state()
{
}

void ThreadPool::adjust()
{
}
// Return true to let pool to handle the thread: reuse or delete.
bool ThreadPool::on_thread_run(ThreadHandle* thread)
{
	// execute one thread with one specified task.
	//
	_LogDebug_("ThreadPool", "run thread handle : %d", thread);

	// Lock the thread attributes.
	ThreadBase* task = NULL;
	{
		// We won't keep this lock during the running of the task.
		// And the task object will be not delete before it's run() has been completed.
		AutoLock(thread->lock);
		task  = thread->task;
	}
	// 
	if (task)
	{
		_LogDebug_("ThreadPoll", "run thread's task: %d", thread->task);
		return task->run();
	}
	else
	{
		_LogDebug_("ThreadPool", "No task to run, send to idle set.");
		// No task for thread, so just return ture, and according to the reuse, pool will reuse it or delete this thread.
		return true;
	}
}
bool ThreadPool::on_thread_finish(ThreadHandle* thread)
{
	_LogDebug_("ThreadPool", "finishing thread handle %d", thread);

	bool need_delete = false;

	// First, lock the thread data. to be save to change it?
	thread->lock.lock();	
	
	if (thread->task)
	{
		// SO the requirement for on_close final state of one thread.
		thread->task->on_close();

		// Delete the task object here, so other modules do not need take care about the allocated task object by themself
		delete thread->task;

		// Set to NULL.
		thread->task = NULL;
	}
	// To know whether need to delete this thread or not.
	need_delete = !thread->reuse;
	// Free the lock.
	thread->lock.unlock();

	// Lock pool attributes.
	_lock.lock();
	// Remove from active set and put to idle set.
	_active_set.erase(thread);
	_idle_set.insert(thread);
	// unlock pool.
	_lock.unlock();

	_LogDebug_("ThreadPool", "finished thread handle %d", thread);

	// return.
	return need_delete;
}

ThreadPool::ThreadHandle* ThreadPool::create_thread(ThreadBase* task)
{
	// Create one thread data object and insert to active set.
	pthread_t thread;
	ThreadHandle* handle = new ThreadHandle();

	// I do not think it needs to be locked, since no other threads could operate on this data object.
	//handle->lock.lock();
	handle->task = task;
	handle->reuse = true;
	::pthread_create(&thread, NULL, &ThreadPool::thread_proc, (void*)handle);
	_LogDebug_("ThreadPool", "Create threading....");
	handle->controller.setup(thread);
	//handle->lock.unlock();
	//
	_LogDebug_("ThreadPool", "Create thread completed");

	// return the object.
	return handle;
}
void ThreadPool::remove_thread(ThreadHandle* thread)
{
	// Remove from the idle set, but the thread data object will be delete after this. 
	_lock.lock();
	_idle_set.erase(thread);
	_LogDebug_("ThreadPool", "Threading is quit!!!!! Handle: %d", thread);
	_lock.unlock();
}

void ThreadPool::kill_idle_thread(unsigned int count)
{
	// Kill such many idle thread, until idle set is empty.
	// Do not call this before you have lock the pool attributes.
	while (count != 0 && !_idle_set.empty())
	{
		// get first one.
		ThreadHandle* handle = *_idle_set.begin();

		// Lock the data.
		handle->lock.lock();
		handle->reuse = false;
		handle->task = NULL;
		_idle_set.erase(_idle_set.begin());

		// put it to active thread, and will go through delete progress.
		_active_set.insert(handle);
		// wake up thread.
		handle->controller.resume();
		// Free the lock.
		handle->lock.unlock();
	}
}

void * ThreadPool::thread_proc(void* param)
{
	ThreadHandle* handle = (ThreadHandle*)param;
	while (true)
	{
		// Check the return value of run(), if return true means task finished it running.
		bool ret = ThreadPool::get_singleton().on_thread_run(handle);
		if (ret)
		{
			// Check whether we need to remove the thread.
			ret = ThreadPool::get_singleton().on_thread_finish(handle);
			if (ret)
			{
				// Now we can quit this thread.
				ThreadPool::get_singleton().remove_thread(handle);
				break;
			}
			else
			{
				// Suspend the thread waiting for next task.
				handle->controller.suspend();
			}
		}
	}
	// Delete the handle;
	delete handle;
	pthread_exit(0);
}

bool ThreadPool::PoolAdjust::run()
{
}
void ThreadPool::PoolAdjust::on_close()
{
}
void ThreadPool::PoolAdjust::shutdown()
{
}
bool ThreadPool::PoolAdjust::is_running()
{
	return true;
}
