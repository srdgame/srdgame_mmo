#include "loginworker.h"
#include "loginsocketbase.h"
#include "loginmgr.h"
#include "packetparser.h"

#include "log.h"
using namespace srdgame;

LoginWorker::LoginWorker(LoginSocketBase* socket) : ThreadBase(), _running(false), _socket(socket)
{
}
LoginWorker::~LoginWorker()
{
	if (is_running())
	{
		shutdown();
	}
}

bool LoginWorker::run()
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

	handle(&p);
	
	return false;
}

void LoginWorker::shutdown()
{
	_running =false;
}
void LoginWorker::on_close()
{
}

bool LoginWorker::is_running()
{
	return _running;
}

void LoginWorker::handle(Packet* packet)
{
	_socket->on_handle(packet);
	packet->free(); // Free the space that we have done.
}

bool LoginWorker::send(Packet* packet)
{
	char sz[MAX_PACKET_LEN];
	::memset(sz, 0, MAX_PACKET_LEN);
	size_t size = PacketParser::get_singleton().to_inter(sz, *packet);
	if (size && _socket && _socket->is_connected())
	{
		return _socket->send(sz, size);
	}
	return false;
}
