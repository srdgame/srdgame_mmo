#include "realmsocket.h"
#include "log.h"
#include "packetparser.h"
#include "realmworker.h"
#include "opcode.h"
#include "rocharinfo.h"
#include "ro_defs.h"
#include "charmgr.h"

using namespace ro;
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
	{	// Char server
		case EC_LOGIN_TO_CHAR:
			{
				LoginToChar* l = (LoginToChar*)packet->param.Data;
				_account_id	 = l->_account;
				// TODO: TO auth.
				//
				// To send feedback.
				Packet p;
				p.op = ES_LOGIN_TO_CHAR;
				p.len = sizeof(p);
				p.param.Int = _account_id;
				this->send_packet(&p);

				// TO send chars.
				RoCharInfo* chars = NULL;
				size_t count = CharMgr::get_singleton().load_chars(_account_id, chars);
				p.op = ES_CHAR_LIST;
				p.len = sizeof(int) + sizeof(RoCharInfo) * count + sizeof(p);
				p.param.Data = new char[sizeof(int) + sizeof(RoCharInfo) * count];
				int* num = (int*)p.param.Data;
				*num = count;
				num++;
				::memcpy((char*)num, (char*)chars, sizeof(RoCharInfo) * count);
				this->send_packet(&p);
				delete [] chars;
				delete [] p.param.Data;
			}
			break;
		case EC_SELECT_CHAR:
			break;
		case EC_CHAR_LIST:
			break;
		case EC_CHAR_CREATE:
			break;
		case EC_CHAR_DELETE:
			break;
		case EC_CHAR_RENAME:
			break;
		case EC_CHAR_KEEP_ALIVE:
			break;
		default:
			break;
	}
}

