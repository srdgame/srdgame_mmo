#include "realmsocket.h"
#include "log.h"
#include "packetparser.h"
#include "realmworker.h"
#include "opcode.h"

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

RealmSocket::RealmSocket()
	: RealmSocketBase(/* using the default buffer size */)
{
	_inter = false;
	_dump_in = true;
	_dump_out = true;
}

RealmSocket::~RealmSocket()
{
	if (_worker && _worker->is_running())
	{
		_worker->shutdown();
	}
}
void RealmSocket::on_send()
{
}

void RealmSocket::on_connect()
{
}

void RealmSocket::on_close()
{
}

void RealmSocket::on_handle(Packet* packet)
{
	_LogDebug_("RealmServer", "Handling one new packet its op : %d", packet->op);
	switch (packet->op)
	{
		case EC_NONE: // For nopacket( :-) just the packet we won't handle it);
			break;
		case EC_INIT:
			break;
		case EC_VERSION: // Provide client version.
			break;
		default:
			break;
	}
}

