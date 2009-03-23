#include "worldworker.h"
#include "packetdefs.h"
#include "worldsocketbase.h"
#include "packetparser.h"
#include "threadpool.h"
#include "autolock.h"

using namespace srdgame;

#define SOCKET_DEBUG

WorldSocketBase::WorldSocketBase() : _worker(NULL), _inter(false)
{
}
WorldSocketBase::~WorldSocketBase()
{
	_worker_lock.lock();
	if (_worker)
	{
		_worker->shutdown();
	}
	_worker_lock.unlock();
}

bool WorldSocketBase::send_packet(Packet* packet)
{
#ifdef SOCKET_DEBUG
	LogDebug("WorldServer", "Starting to send packet, opcode: %d", packet->op);
#endif
	// TODO: To refine this send, the buffer allocation is useless here, we could use send_buf here.
	char sz[MAX_PACKET_LEN];
	::memset(sz, 0, MAX_PACKET_LEN);
	size_t size = 0;
	if (_inter)
	{
		size = PacketParser::get_singleton().to_inter(sz, *packet);
	}
	else
	{
		size = PacketParser::get_singleton().to_ex(sz, *packet);
	}

#ifdef SOCKET_DEBUG
	LogDebug("WorldServer", "Packet length is : %d", size);
#endif
	if (size && is_connected())
	{
		return this->send(sz, size);
	}
#ifdef SOCKET_DEBUG
	LogError("WorldServer", "Socket is't been connected");
#endif
	return false;
}

void WorldSocketBase::start_worker()
{
	AutoLock lock(_worker_lock);
	if (_worker)
		return;

	LogDebug("WorldServer", "Starting worker thread");
	_worker = new WorldWorker(this);
	ThreadPool::get_singleton().execute(_worker);
}
