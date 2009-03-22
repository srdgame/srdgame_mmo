#include "realmsocket.h"
#include "log.h"
#include "packetparser.h"
#include "realmworker.h"
#include "opcode.h"
#include "rocharinfo.h"
#include "ro_defs.h"
#include "rosql.h"
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
	for( size_t i = 0; i < _chars.size(); ++i)
	{
		RoCharInfo* info = _chars[i];
		delete info;
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
				_LogDebug_("RealmServer", "Account is connected, id : %d", l->_account);
				_account_id	 = l->_account;
				// TODO: TO auth.
				//
				// To send feedback.
				Packet p;
				p.op = ES_LOGIN_TO_CHAR;
				p.len = sizeof(p);
				p.param.Int = _account_id;
				this->send_packet(&p);

				//usleep(10000);
				// TO send chars.
				RoCharInfo* chars;
				size_t count = CharMgr::get_singleton().load_chars(_account_id, chars);
				_LogDebug_("RealmServer", "Characters has been loaded, count is : %d, chars : %d", count, chars);
				for (size_t i = 0; i < count; ++i)
				{
					_chars.push_back(chars + i);
				}
				p.op = ES_CHAR_LIST;
				p.len = sizeof(Packet) + sizeof(CharDataList);
				CharDataList* list = new CharDataList();
				list->_count = count;
				list->_chars = chars;
				p.param.Data = (char*)list;
				this->send_packet(&p);
				//delete [] chars;
				delete [] p.param.Data;
			}
			break;
		case EC_SELECT_CHAR:
			break;
		case EC_CHAR_LIST:
			break;
		case EC_CHAR_CREATE:
			{
				CreateCharData* data = (CreateCharData*)packet->param.Data;
				if (!data)
					break;
				RoCharInfo* char_info = CharMgr::get_singleton().create_new(data, _account_id);
				if (!char_info)
				{
					Packet p;
					p.op = ES_CHAR_CREATE;
					p.len = sizeof(Packet);
					p.param.Char = 1;
					LogError("RealmServer", "Create character failed!!!");
					send_packet(&p);
				}
				else
				{
					_chars.push_back(char_info);
					Packet p;
					p.op = ES_CHAR_CREATE;
					p.len = sizeof(RoCharInfo) + sizeof(Packet);
					p.param.Data = (char*)char_info;
					LogSuccess("RealmServer", "Success to create character with name %s", data->_name);
					send_packet(&p);
				}
			}
			break;
		case EC_CHAR_DELETE:
			break;
		case EC_CHAR_RENAME:
			break;
		case EC_CHAR_KEEP_ALIVE:
			{
				Packet p;
				p.len = sizeof(Packet);
				p.op = ES_CHAR_KEEP_ALIVE;
				p.param.Int = packet->param.Int;
				this->send_packet(&p);
			}
			break;
		default:
			break;
	}
}

