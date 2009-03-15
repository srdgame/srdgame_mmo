#include "intersocket.h"
#include "log.h"
#include "packetparser.h"
#include "loginworker.h"
#include "opcode.h"
#include "loginauth.h"

using namespace srdgame;
using namespace srdgame::opcode;

#ifndef SOCKET_DEBUG
#define SOCKET_DEBUG
#undef _LogDebug_
#endif

#ifdef SOCKET_DEBUG
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

InterSocket::InterSocket()
	: LoginSocketBase(/* using the default buffer size */)
{

}

InterSocket::~InterSocket()
{
	if (_worker && _worker->is_running())
	{
		_worker->shutdown();
	}
}
void InterSocket::on_send()
{
}

void InterSocket::on_connect()
{
}

void InterSocket::on_close()
{
}

void InterSocket::on_handle(Packet* packet)
{
	_LogDebug_("LoginServer", "Handling one new packet its op : %d", packet->op);
	switch (packet->op)
	{
		case I_PING:
			LogDebug("LoginServer", "I_PING");
			packet->param.Int++;
			this->send(packet);
			break;
		case I_OFFLINE:
			LogDebug("LoginServer", "I_OFFLINE");
			LoginMgr::get_singleton().remove_realm_server(_socket);
			break;
		case I_NOTIFY:
			LogDebug("LoginServer", "I_NOTIFY");
			{
				if (packet->param.Long != 1)
				{
					LogError("LoginServer", "Not a login server are trying to connect us!!!");
					_socket->close();
					break;
				}
				LoginMgr::get_singleton().add_realm_server(_socket);
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

			// TODO: Ask for info?
			break;
		case IC_NAME:
			LogDebug("LoginServer", "IC_NAME");
			{
				int size = PacketParser::get_ex_len(*packet);
				if (size > 0)
				{
					char* sz = new char[size + 1];
					::memset(sz, 0, size + 1);
					::memcpy(sz, packet->param.Data, size);
					LoginMgr::get_singleton().update_realm_server_name(_socket, std::string(sz));
					delete[] sz;
				}
			}
			break;
		case IC_STATUS:
		case IC_POST_STATUS:
			LogDebug("LoginServer", "IC_STATUS || IC_POST_STATUS");
			{
				LoginSrvStatus status = (LoginSrvStatus)packet->param.Long;
				LoginMgr::get_singleton().update_realm_server_status(_socket, status);

			}
			break;
		case IC_INFO:
			LogDebug("LoginServer", "IC_INFO");
			{
				int size = PacketParser::get_ex_len(*packet);
				if (size > 0)
				{
					char* sz = new char[size + 1];
					::memset(sz, 0, size + 1);
					::memcpy(sz, packet->param.Data, size);
					LoginMgr::get_singleton().update_realm_server_info(_socket, std::string(sz));
					delete[] sz;
				}
			}
			break;
		default:
			break;
	}
}

