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
	shutdown();
}
void ThreadPool::init(unsigned int cap)
{
	_LogDebug_("ThreadPool", "Init the pool as capicity: %d", cap);
	AutoLock lock(_lock);
	if (_inited)
		return;

	ThreadHandle* handle = NULL;
	while (cap != 0)
	{
		_LogDebug_("Threadpool", "Create one more thread");
		handle = create_thread(NULL);
		_active_set.insert(handle);
		--cap;
	}
	_inited = true;
	_LogDebug_("ThreadPool", "Init has completed");
}

void ThreadPool::shutdown()
{
	LogNotice("ThreadPool", "Shutdown all the threads");
	_lock.lock();
	if (!_inited)
	{
		_lock.unlock();
		return;
	}
	
	// send close message to thread.
	_LogDebug_("ThreadPool", "Sending closing signal to ative threads: %d", _active_set.size());
	for (ThreadSet::iterator ptr = _active_set.begin(); ptr != _active_set.end(); ++ptr)
	{
		ThreadHandle* handle = *ptr;
		handle->lock.lock();
		handle->reuse = false;
		handle->lock.unlock();
		if (handle->task)
		{
			handle->task->shutdown();
		}
	}
	// set the reuse to false, and active the thread, then it will be deleted.
	this->kill_idle_thread((unsigned int)_idle_set.size());

	_lock.unlock();

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

/*
void ThreadPool::close_thread(ThreadHandle*)
{

}
*/
void ThreadPool::execute(ThreadBase* task)
{
	_LogDebug_("ThreadPool", "Execute task....");
//	LogDebug("", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
	ThreadHandle* handle = NULL;
	_lock.lock();
	if (!_idle_set.empty())
	{
		handle = *_idle_set.begin();
		_idle_set.erase(_idle_set.begin());

		handle->task = task;
		_LogDebug_("ThreadPool", "Reuse thread handle: %d", handle);
		handle->controller.resume();
	}
	else
	{
		_LogDebug_("ThreadPool", "No more free thread, creating...");
		handle = create_thread(task);
	}
	_active_set.insert(handle);
	_lock.unlock();
	_LogDebug_("ThreadPool", "Task has been put it run queue");
}

void ThreadPool::print_state()
{
}

void ThreadPool::adjust()
{
}
bool ThreadPool::on_thread_run(ThreadHandle* thread)
{
	_LogDebug_("ThreadPool", "run thread handle : %d", thread);
	AutoLock(thread->lock);
	if (thread->task)
	{
		_LogDebug_("ThreadPoll", "run thread's task: %d", thread->task);
		return thread->task->run();
	}
	else
	{
		_LogDebug_("ThreadPool", "No task to run");
		return true;
	}
}
bool ThreadPool::on_thread_finish(ThreadHandle* thread)
{
	_LogDebug_("ThreadPool", "finishing thread handle %d", thread);

	bool need_delete = false;

	thread->lock.lock();
	
	
	if (thread->task)
	{
		thread->task->on_close();
		delete thread->task;
	}
	thread->task = NULL;
	need_delete = !thread->reuse;

	thread->lock.unlock();

	_lock.lock();
	_active_set.erase(thread);
	_idle_set.insert(thread);
	_lock.unlock();

	_LogDebug_("ThreadPool", "finished thread handle %d", thread);

	return need_delete;
}

ThreadPool::ThreadHandle* ThreadPool::create_thread(ThreadBase* task)
{
	pthread_t thread;
	ThreadHandle* handle = new ThreadHandle();
	
	handle->lock.lock();
	handle->task = task;
	handle->reuse = true;
	::pthread_create(&thread, NULL, &ThreadPool::thread_proc, (void*)handle);
	_LogDebug_("ThreadPool", "Create threading....");
	handle->controller.setup(thread);
	handle->lock.unlock();
	_LogDebug_("ThreadPool", "Create thread completed");
	return handle;
}
void ThreadPool::remove_thread(ThreadHandle* thread)
{
	_lock.lock();
	_idle_set.erase(thread);
	_LogDebug_("ThreadPool", "Threading is quit!!!!! Handle: %d", thread);
	_lock.unlock();
}

void ThreadPool::kill_idle_thread(unsigned int count)
{
	while (count != 0 && !_idle_set.empty())
	{
		ThreadHandle* handle = *_idle_set.begin();
		handle->lock.lock();
		handle->reuse = false;
		handle->task = NULL;
		_idle_set.erase(_idle_set.begin());
		_active_set.insert(handle);
		handle->controller.resume();
		handle->lock.unlock();
	}
}

void * ThreadPool::thread_proc(void* param)
{
	ThreadHandle* handle = (ThreadHandle*)param;
	while (true)
	{
		bool ret = ThreadPool::get_singleton().on_thread_run(handle);
		if (ret)
		{
			ret = ThreadPool::get_singleton().on_thread_finish(handle);
			if (ret)
			{
				// Now we can quit this thread.
				ThreadPool::get_singleton().remove_thread(handle);
				break;
			}
			else
			{
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
