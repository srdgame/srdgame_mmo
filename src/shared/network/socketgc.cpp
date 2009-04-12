#include "socketgc.h"
#include "autolock.h"
#include "timedefs.h"
#include "threadpool.h"
#include "socket.h"
#include "log.h"

using namespace srdgame;

SocketGC::SocketGC() : _closing(false)
{

}
SocketGC::~SocketGC()
{
	_closing = true;
	GCData data;
	while (_data.try_pop(data))
	{
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
			_worker = new GCWorker();
			ThreadPool::get_singleton().execute(_worker);
		}
	}
}
bool SocketGC::GCWorker::run()
{
	GCData data;
	if (!SocketGC::get_singleton()._data.try_pop(data))
	{
		if (_closing)
		{
			return true;
		}

		usleep(1000000);
		return false;
	}
	
	if (data._time > gettick())
		return false; // Time is not enough.

	LogDebug("Socket GC", "Deleting one socket------------------");
	delete data._socket;
	return false;// Process Next data.
}
