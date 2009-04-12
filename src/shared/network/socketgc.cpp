#include "socketgc.h"
#include "autolock.h"
#include "timedefs.h"
#include "threadpool.h"
#include "socket.h"
#include "log.h"

using namespace srdgame;

SocketGC::SocketGC() : _closing(false), _worker(NULL)
{
	
}
SocketGC::~SocketGC()
{
	_closing = true;
	GCData data;
	while (_data.try_pop(data))
	{
		LogSuccess("Socket GC", "Deleteing sockets objects from Destructor");
		delete data._socket;
	}
}

void SocketGC::GC(Socket* s)
{
	GCData data;
	data._socket = s;
	data._time = gettick() + 2000;
	_data.push(data);
	{
		if (_worker)
		{
			return;
		}

		if (_worker_lock.trylock())
		{
			LogSuccess("Socket GC", "Start GC threading");
			_worker = new GCWorker();
			_worker_lock.unlock();
			ThreadPool::get_singleton().execute(_worker);
		}
	}
}
bool SocketGC::GCWorker::run()
{
	static GCData data;
	static bool b_has = false;
	//LogDebug("Socket GC", "Running---------------------");
	if (!b_has)
	{
		if (!SocketGC::get_singleton()._data.try_pop(data))
		{
			if (_closing)
			{
				return true;
			}
			//LogDebug("Socket GC", "Sleeping for time----------");
			usleep(10000);
			return false;
		}
	}
	b_has = true;
	if (data._time > gettick())
	{
		//LogDebug("Socket GC", "Next loop for time");
		return false; // Time is not enough.
	}

	b_has = false;
	//LogDebug("Socket GC", "Deleting one socket------------------");
	delete data._socket;
	return false;// Process Next data.
}
