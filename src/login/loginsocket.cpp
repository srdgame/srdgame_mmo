#include "loginsocket.h"
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

LoginSocket::LoginSocket()
	: LoginSocketBase(/* using the default buffer size */)
{
	_inter = false;
	_dump_in = true;
	_dump_out = true;
}

LoginSocket::~LoginSocket()
{

}
void LoginSocket::on_send()
{
}

void LoginSocket::on_connect()
{
}

void LoginSocket::on_close()
{
}

void LoginSocket::on_handle(Packet* packet)
{
	// Lock the worker_lock for save.
	_LogDebug_("LoginServer", "Handling one new packet its op : %d", packet->op);
	switch (packet->op)
	{
		case EC_NONE: // For nopacket( :-) just the packet we won't handle it);
			break;
		case EC_INIT:
			break;
		case EC_VERSION: // Provide client version.
			break;

		// For login
		case EC_LOGIN:
			_LogDebug_("LoginServer", "EC_LOGIN");
			LoginAuth::get_singleton().handle_login(this, packet);
			break;
		case EC_LOGOUT:
			break;
		case EC_GET_LS_INFO:
				_LogDebug_("LoginServer", "EC_GET_LS_INFO");
				break;
		case EC_ADMIN_LOGIN:
			LoginAuth::get_singleton().handle_admin_login(this, packet);
			break;

		// For server list not only for login.
		case EC_SERVER_LIST:	
			_LogDebug_("LoginServer", "EC_SERVER_LIST");
			break;
		case EC_ENTER_SERVER:
			_LogDebug_("LoginServer", "EC_ENTER_SERVER");
			break;

		case EC_CRYPTO_KEY:	
			_LogDebug_("LoginServer", "EC_CRYPTO_KEY");
			LoginAuth::get_singleton().get_crypto_key(this, packet);
			break;

		default:
			break;
	}
}

