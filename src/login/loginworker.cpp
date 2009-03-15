#include "loginworker.h"
#include "loginsocket.h"
#include "loginmgr.h"
#include "packetparser.h"

#include "log.h"
using namespace srdgame;

LoginWorker::LoginWorker(LoginSocketBase* socket) : ThreadBase(), _running(true), _socket(socket)
{
	LogDebug("LoginServer", "Construtor of LoginWorker");
}
LoginWorker::~LoginWorker()
{
	LogDebug("LoginServer", "Destructor of LoginWorker");
	//if (is_running())
	{
		shutdown();
	}
}

bool LoginWorker::run()
{
	LogDebug("LoginServer", "LoginWorker:run()");
	
	// return true to delete this object.
	// return false run() will be called again.
	// process the data until empty.
	if (!_socket || !_running)
	{
		LogDebug("LoginServer", "No socket or is closing, QUIT!!!");
		return true;
	}
	if (_socket->_worker != this)
	{
		LogDebug("LoginServer", "The worker is me, I have to QUIT!!!");
		return true;
	}

	Packet p;
	if (!_socket->_packets.try_pop(p) || !_running)
	{
		// no data 
		LogDebug("LoginServer", "LoginWorker finished his job");
		_socket->_worker_lock.lock();
		_socket->_worker = NULL;
		_socket->_worker_lock.unlock();
		LogDebug("LoginServer", "LoginWorker killed the _worker");
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
	PacketParser::free(*packet); // Free the space that we have done.
}


