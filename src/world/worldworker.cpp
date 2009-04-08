#include "worldworker.h"
#include "worldsocketbase.h"
#include "worldmgr.h"
#include "packetparser.h"

#include "log.h"
using namespace srdgame;

//#define _DEBUG_WORKER_

#ifdef _DEBUG_WORKER_
#undef _LogDebug_
#define _LogDebug_ LogDebug
#else
#undef _LogDebug_
#define _LogDebug_ //
#endif

WorldWorker::WorldWorker(WorldSocketBase* socket) : ThreadBase(), _running(true), _socket(socket)
{
}
WorldWorker::~WorldWorker()
{
	if (is_running())
	{
		shutdown();
	}
}

bool WorldWorker::run()
{
	_LogDebug_("WorldServer", "WorldWorker::run()!!!!!!!!!!!!!!!!!!!!!!!");
	
	// return true to delete this object.
	// return false run() will be called again.
	// process the data until empty.
	if (!_socket || !_running)
	{
		_LogDebug_("WorldServer", "Quiting World Worker thread");
		_socket->_worker_lock.lock();
		_socket->_worker = NULL;
		_socket->_worker_lock.unlock();
		return true;
	}

	if (_socket->_worker != this)
		return true;

	Packet p;
	if (!_socket->_packets.try_pop(p) || !_running)
	{
		_LogDebug_("WorldServer", "World Worker finished its job!!!");
		// no data, we will quit this.
		_socket->_worker_lock.lock();
		_socket->_worker = NULL;
		_socket->_worker_lock.unlock();
		return true;
	}

	_LogDebug_("WorldServer", "World Worker asking socket to process one packet");
	handle(&p);
	
	return false;
}

void WorldWorker::shutdown()
{
	_running =false;
}
void WorldWorker::on_close()
{
}

bool WorldWorker::is_running()
{
	return _running;
}

void WorldWorker::handle(Packet* packet)
{
	_socket->on_handle(packet);
	PacketParser::free(*packet); // Free the space that we have done.
}

