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
		_queues[i]._queue = new FastQueue<Waitor*>();
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

void WorldUpdater::add(Waitor* w)
{
	AutoLock lock(_lock);
	if (w->get_priority() > NP_COUNT)
		return;
	_queues[w->get_priority()]._queue->push(w);

}
void WorldUpdater::remove(Waitor* w)
{
	AutoLock lock(_lock);
	_removing.insert(make_pair< Waitor*, bool >(w, true));
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
	Waitor* w;
	if (_queues[_doing]._queue->try_pop(w))
	{
		if (_removing.find(w) != _removing.end())
			return;// Remove the wect.

		w->notify(gettick());

		// adjust the priority.
		int pri = w->get_priority();
		if (_doing == pri)
		{
			_new_queue->push(w);
		}
		else if (pri != NP_REMOVE_ME)
		{
			_queues[pri]._queue->push(w);
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
		_new_queue = new FastQueue<Waitor*>();
	}
}

