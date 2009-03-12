#include "realmworker.h"
#include "realmsocket.h"
#include "realmmgr.h"
#include "packetparser.h"

#include "log.h"
using namespace srdgame;

RealmWorker::RealmWorker(RealmSocket* socket) : ThreadBase(), _running(true), _socket(socket)
{
	LogDebug("RealmServer", "Construtor of RealmWorker");
}
RealmWorker::~RealmWorker()
{
	LogDebug("RealmServer", "Destructor of RealmWorker");
	//if (is_running())
	{
		shutdown();
	}
}

bool RealmWorker::run()
{
	LogDebug("RealmServer", "RealmWorker:run()");
	
	// return true to delete this object.
	// return false run() will be called again.
	// process the data until empty.
	if (!_socket || !_running)
	{
		LogDebug("RealmServer", "No socket or is closing, QUIT!!!");
		return true;
	}
	if (_socket->_worker != this)
	{
		LogDebug("RealmServer", "The worker is me, I have to QUIT!!!");
		return true;
	}

	Packet p;
	if (!_socket->_packets.try_pop(p) || !_running)
	{
		// no data 
		LogDebug("RealmServer", "RealmWorker finished his job");
		_socket->_worker_lock.lock();
		_socket->_worker = NULL;
		_socket->_worker_lock.unlock();
		LogDebug("RealmServer", "RealmWorker killed the _worker");
		return true;
	}

	handle(&p);
	
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

void RealmWorker::handle(Packet* packet)
{
	// One data got.
	// Pcessing data
	//
	if (!_socket)
	{
		PacketParser::free(*packet);
		return;
	}
	switch (packet->op)
	{
		case I_PING:
			LogDebug("RealmServer", "I_PING");
			packet->param.Int++;
			this->send(packet);
			break;
		case I_OFFLINE:
			LogDebug("RealmServer", "I_OFFLINE");
			if (_socket->_inter)
			{
				RealmMgr::get_singleton().remove_login_server(_socket);
			}
			else
			{
				RealmMgr::get_singleton().remove_client(_socket);
			}
			break;
		case I_NOTIFY:
			LogDebug("RealmServer", "I_NOTIFY");
			if (_socket->_inter)
			{
				if (packet->param.Long != 1)
				{
					LogError("RealmServer", "Not a login server are trying to connect us!!!");
					_socket->close();
					break;
				}
				RealmMgr::get_singleton().add_login_server(_socket);
				// send ask name and info status packets.
				Packet p;
				p.op = IS_GET_NAME;
				p.len = sizeof(p);
				this->send(&p);

				p.op = IS_GET_INFO;
				this->send(&p);

				p.op = IS_GET_STATUS;
				this->send(&p);
			}
			else
			{
				RealmMgr::get_singleton().add_client(_socket);
			}

			// TODO: Ask for info?
			break;
		case IC_NAME:
			LogDebug("RealmServer", "IC_NAME");
			if (_socket->_inter)
			{
				int size = PacketParser::get_ex_len(*packet);
				if (size > 0)
				{
					char* sz = new char[size + 1];
					::memset(sz, 0, size + 1);
					::memcpy(sz, packet->param.Data, size);
					RealmMgr::get_singleton().update_login_server_name(_socket, std::string(sz));
					delete[] sz;
				}
			}
			break;
		case IC_STATUS:
		case IC_POST_STATUS:
			LogDebug("RealmServer", "IC_STATUS || IC_POST_STATUS");
			if (_socket->_inter)
			{
				LoginSrvStatus status = (LoginSrvStatus)packet->param.Long;
				RealmMgr::get_singleton().update_login_server_status(_socket, status);

			}
			break;
		case IC_INFO:
			LogDebug("RealmServer", "IC_INFO");
			if (_socket->_inter)
			{
				int size = PacketParser::get_ex_len(*packet);
				if (size > 0)
				{
					char* sz = new char[size + 1];
					::memset(sz, 0, size + 1);
					::memcpy(sz, packet->param.Data, size);
					RealmMgr::get_singleton().update_login_server_info(_socket, std::string(sz));
					delete[] sz;
				}
			}
			break;
		default:
			break;
	}
	
	PacketParser::free(*packet); // Free the space that we have done.
}

bool RealmWorker::send(Packet* packet)
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
