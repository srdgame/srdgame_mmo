#include "realmsocket.h"
#include "log.h"
#include "packetparser.h"
#include "realmworker.h"
#include "opcode.h"
#include "rocharinfo.h"
#include "ro_defs.h"
#include "rosql.h"
#include "charmgr.h"
#include "typedefs.h"
#include "strlib.h"
#include "realmmgr.h"

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
	_dump_in = false;
	_dump_out = false;
}

RealmSocket::~RealmSocket()
{
	if (_worker && _worker->is_running())
	{
		_worker->shutdown();
	}
	clean_chars();	
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
				clean_chars();

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
				RoCharInfoBase* chars;
				size_t count = CharMgr::get_singleton().load_chars(_account_id, chars);
				_LogDebug_("RealmServer", "Characters has been loaded, count is : %d, chars : %d", count, chars);
				for (size_t i = 0; i < count; ++i)
				{
					_chars[chars[i]._slot] = chars[i]._id;
				}
				p.op = ES_CHAR_LIST;
				p.len = sizeof(Packet) + sizeof(CharDataList);
				CharDataList* list = new CharDataList();
				list->_count = count;
				list->_chars = chars;
				p.param.Data = (char*)list;
				this->send_packet(&p);
				delete list;
				delete [] chars;
				//delete [] p.param.Data;
			}
			break;
		case EC_SELECT_CHAR:
			{
				int slot = packet->param.Int;
				bool b_ok = false;
				_LogDebug_("RealmServer", "Select character!!!!! Slot: %d, \t chars_size : %d", slot, (int) _chars.size());
				int char_id = _chars[slot];
				if (char_id != 0)
				{
					RoCharInfo char_info;
					if (!CharMgr::get_singleton().load_char_detail(char_id, char_info))
							break;

						_LogDebug_("RealmServer", "HAS FOUND THE CHAR===============================");
						Packet p;
						p.op = ES_SELECT_CHAR;
						p.len = sizeof(Packet) + sizeof(MapServerInfo);
						MapServerInfo info;
						info._account = _account_id;
						info._ip = 0;
						info._port = 0;
						memset(info._map_name, 0, MAX_MAP_NAME_LEN);
						memcpy(info._map_name, char_info._last_pos._map_name, MAX_MAP_NAME_LEN);
						strcat(info._map_name, ".gat");
						// Get world server info;
						WorldSrvInfo server;
						_LogDebug_("RealmServer", "Char is now in map : %s", char_info._last_pos._map_name);
						if (RealmMgr::get_singleton().get_server_by_map(string(char_info._last_pos._map_name), server))
						{
							_LogDebug_("RealmServer", "Got map~!!!!!!!!!!!!!!!!!!!!");
							info._ip = str2ip(server.ip);
							info._port = (uint16)server.port;
							p.param.Data = (char*) &info;
							send_packet(&p);
							b_ok = true;
							break;
						}
				}
				
				{
					Packet p;
					p.op = ES_SELECT_CHAR;
					p.len = sizeof(Packet);
					p.param.Int = 1;
					send_packet(&p);
				}
			}
			break;
		case EC_CHAR_LIST:
			break;
		case EC_CHAR_CREATE:
			{
				CreateCharData* data = (CreateCharData*)packet->param.Data;
				if (!data)
					break;
				RoCharInfoBase* char_info = CharMgr::get_singleton().create_new(data, _account_id);
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
					_chars[char_info->_slot] = char_info->_id;
					Packet p;
					p.op = ES_CHAR_CREATE;
					p.len = sizeof(RoCharInfoBase) + sizeof(Packet);
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
void RealmSocket::clean_chars()
{

	_chars.clear();
}
