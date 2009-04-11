#include "worldupdater.h"
#include "timedefs.h"
#include "autolock.h"
#include "threadpool.h"

using namespace srdgame;
using namespace std;

WorldUpdater::Thread::Thread(WorldUpdater* owner) : _owner(owner), _closing(false)
{

}
WorldUpdater::Thread::~Thread()
{
}
bool WorldUpdater::Thread::run()
{
	if (_closing)
		return true;
	_owner->update();
	return false;
}
void WorldUpdater::Thread::shutdown()
{
	_closing = true;
}
void WorldUpdater::Thread::on_close()
{
}
bool WorldUpdater::Thread::is_running()
{
	return !_closing;
}
WorldUpdater::WorldUpdater() : _doing (-1), _new_queue(NULL), _pause (false)
{
	for (int i = 0; i < NP_COUNT; ++i)
	{
		_queues[i]._queue = new FastQueue<NObject*>();
	}
}
WorldUpdater::~WorldUpdater()
{
}
void WorldUpdater::start()
{
	AutoLock lock(_lock);
	_pause = false;
	if (_thread)
		return;
	_thread = new Thread(this);
	ThreadPool::get_singleton().execute(_thread);
}

void WorldUpdater::pause()
{
	AutoLock lock(_lock);
	_pause = true;
}

void WorldUpdater::stop()
{
	AutoLock lock(_lock);
	if (_thread)
	{
		_thread->shutdown();
		_thread = NULL;
	}
}

void WorldUpdater::add(NObject* obj)
{
	AutoLock lock(_lock);
	if (obj->get_priority() > NP_COUNT)
		return;
	_queues[obj->get_priority()]._queue->push(obj);

}
void WorldUpdater::remove(NObject* obj)
{
	AutoLock lock(_lock);
	_removing.insert(make_pair< NObject*, bool >(obj, true));
}

void WorldUpdater::update()
{
	AutoLock lock(_lock);
	if (_pause)
		return;

	select();

	if (_doing == -1)
	{
		return;
	}
	NObject* obj;
	if (_queues[_doing]._queue->try_pop(obj))
	{
		if (_removing.find(obj) != _removing.end())
			return;// Remove the object.

		obj->notify(gettick());

		// adjust the priority.
		int pri = obj->get_priority();
		if (_doing == pri)
		{
			_new_queue->push(obj);
		}
		else
		{
			_queues[pri]._queue->push(obj);
		}
	}
	else
	{
		// If finished current queue.
		delete _queues[_doing]._queue;
		_queues[_doing]._queue = _new_queue;
		_queues[_doing]._delay = 0;
		_new_queue = NULL;
	}
}

void WorldUpdater::select()
{
	if (_doing != -1)
		return;
	assert(_new_queue == NULL);
	int delay = 0;
	for (int i = 0; i < NP_COUNT; i++)
	{
		if (_queues[i]._queue->get_size() == 0)
			continue;

		int i_delay = _queues[i]._delay * (NP_COUNT - i);
		if (i_delay > delay)
		{
			delay = i_delay;
			_doing = i;
		}
	}
	if (_doing != -1)
	{
		_new_queue = new FastQueue<NObject*>();
	}
}

