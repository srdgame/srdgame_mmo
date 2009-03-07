#include "realmworker.h"
#include "realmsocket.h"

#include "log.h"
using namespace srdgame;

RealmWorker::RealmWorker() : ThreadBase(), _running(false), _socket(NULL)
{
}
RealmWorker::~RealmWorker()
{
	if (is_running())
	{
		shutdown();
	}
}

bool RealmWorker::run()
{
	
	// return true to delete this object.
	// return false run() will be called again.
	// process the data until empty.
	if (!_socket || !_running)
		return true;

	if (_socket->_worker != this)
		return true;

	Packet p;
	if (!_socket->_packets.try_pop(p) || !_running)
	{
		// no data 
		_socket->_worker = NULL;
		return true;
	}
	// One data got.
	// Pcessing data
	p.free(); // Free the space that we have done.

	return false;
}

void RealmWorker::shutdown()
{
	_running =false;
}
void RealmWorker::on_close()
{
}

bool RealmWorker::is_running()
{
	return _running;
}
