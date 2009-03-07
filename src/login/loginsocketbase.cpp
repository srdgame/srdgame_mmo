#include "loginworker.h"
#include "packetdefs.h"
#include "loginsocketbase.h"

using namespace srdgame;

LoginSocketBase::LoginSocketBase() : _worker(NULL)
{
}
LoginSocketBase::~LoginSocketBase()
{
	_worker_lock.lock();
	if (_worker)
	{
		_worker->shutdown();
	}
	_worker_lock.unlock();
}


