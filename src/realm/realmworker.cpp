#include "realmworker.h"
#include "realmsocket.h"
#include "realmmgr.h"
#include "packetparser.h"

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
		packet->free();
		return;
	}
	switch (packet->op)
	{
		case I_PING:
			packet->param.Int++;
			this->send(packet);
			break;
		case I_OFFLINE:
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
			if (_socket->_inter)
			{
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
			if (_socket->_inter)
			{
				int size = packet->get_ex_len();
				if (size > 0)
				{
					char* sz = new char[size + 1];
					::memset(sz, 0, size + 1);
					::memcpy(sz, packet->param.Data, size);
					RealmMgr::get_singleton().update_login_server_name(_socket, std::string(sz));
				}
			}
			break;
		case IC_STATUS:
		case IC_POST_STATUS:
			if (_socket->_inter)
			{
				LoginSrvStatus status = (LoginSrvStatus)packet->param.Long;
				RealmMgr::get_singleton().update_login_server_status(_socket, status);

			}
			break;
		case IC_INFO:
			if (_socket->_inter)
			{
				int size = packet->get_ex_len();
				if (size > 0)
				{
					char* sz = new char[size + 1];
					::memset(sz, 0, size + 1);
					::memcpy(sz, packet->param.Data, size);
					RealmMgr::get_singleton().update_login_server_info(_socket, std::string(sz));
				}
			}
			break;
		default:
			break;
	}
	
	packet->free(); // Free the space that we have done.
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
