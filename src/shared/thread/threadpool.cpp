#include "threadpool.h"

using namespace srdgame;

ThreadPool::ThreadPool() : _inited(false)
{
}

void ThreadPool::init(unsigned int cap)
{
	AutoLock(_lock) lock;
	if (_inited)
		return;

	ThreadHandle* handle = NULL;
	while (cap != 0)
	{
		handle = create_thread(NULL);
		_active_set.insert(handle);
		--cap;
	}
}

void ThreadPool::shutdown()
{
	_lock.lock();
	if (!_inited)
	{
		_lock.unlock();
		return;
	}
	// send close message to thread.
	for (ThreadSet::iterator ptr = _active_set.begin(); ptr != _active_set.end(); ++ptr)
	{
		ThreadHandle* handle = *ptr;
		handle->lock.lock();
		handle->reuse = false;
		handle->lock.unlock();
		handle->exit();
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
			_lock.unlcok();
			break;
		}
		_lock.unlock();
	}
}

/*
void ThreadPool::close_thread(ThreadHandle*)
{

}
*/
void ThreadPool:execute(ThreadBase* task)
{
	ThreadHandle* handle = NULL;
	_lock.lock();
	if (_idle_set.size() == 0)
	{
		handle = *_idle_set.begin();
		_idle_set.earse(_idle_set.begin());

		handle->task = task;
		handle->controller.resume();
	}
	else
	{
		handle = create_thread(task);
	}
	_active_set.insert(handle)
	_lock.unlock();
}

void ThreadPool::print_state()
{
}

void ThreadPool::adjust()
{
}
bool ThreadPool::on_thread_run(ThreadHandle* thread)
{
	AutoLock(thread->lock);
	if (thread->task)
	{
		return thread->task->run();
	}
	else
	{
		return true;
	}
}
bool ThreadPool::on_thread_finish(ThreadHandle* thread)
{
	bool need_delete = false;

	thread->lock.lock();
	
	thread->task->on_close();
	delete thread->task;
	thread->task = NULL;
	need_delete = !thread->reuse;

	thread->lock.unlock();

	_lock.lock();
	_active_set.erase(thread);
	_idle_set.insert(thread);
	_lock.unlock();

	return need_delete;
}

ThreadHandle* ThreadPool::create_thread(ThreadBase* task)
{
	pthread_t thread;
	ThreadHandle* handle = new ThreadHandle();
	
	handle->lock.lock();
	handle->task = task;
	pthread_create(&thread, NULL, &ThreadPool::thread_proc, (void*)handle);
	handle->controller.bind(thread);
	handle->lock.unlock();

	return handle;
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

