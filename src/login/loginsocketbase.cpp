#include "loginworker.h"
#include "packetdefs.h"
#include "loginsocketbase.h"
#include "packetparser.h"
#include "threadpool.h"

using namespace srdgame;

#define SOCKET_DEBUG

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

bool LoginSocketBase::send_packet(Packet* packet)
{
#ifdef SOCKET_DEBUG
	LogDebug("LoginServer", "Starting to send packet, opcode: %d", packet->op);
#endif
	// TODO: To refine this send, the buffer allocation is useless here, we could use send_buf here.
	char sz[MAX_PACKET_LEN];
	::memset(sz, 0, MAX_PACKET_LEN);
	size_t size = PacketParser::get_singleton().to_inter(sz, *packet);

#ifdef SOCKET_DEBUG
	LogDebug("LoginServer", "Packet length is : %d", size);
#endif
	if (size && is_connected())
	{
		return this->send(sz, size);
	}
#ifdef SOCKET_DEBUG
	LogError("LoginServer", "Socket is't been connected");
#endif
	return false;
}

void LoginSocketBase::start_worker()
{
	LogDebug("LoginServer", "Starting worker thread");
	if (_worker)
		return;
	_worker_lock.lock();
	_worker = new LoginWorker(this);
	ThreadPool::get_singleton().execute(_worker);
	_worker_lock.unlock();
}
