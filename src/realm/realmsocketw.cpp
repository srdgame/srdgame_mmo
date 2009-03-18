#include "realmsocketw.h"
#include "log.h"
#include "packetparser.h"
#include "realmworker.h"
#include "realmmgr.h"

using namespace srdgame;

RealmInterSocketW::RealmInterSocketW()
	: RealmSocketBase()
{
}

RealmInterSocketW::~RealmInterSocketW()
{
	
}

void RealmInterSocketW::on_send()
{
}

void RealmInterSocketW::on_connect()
{
}

void RealmInterSocketW::on_close()
{
	LogDebug("RealmServer", "connection with world server has been dropdown");
}
void RealmInterSocketW::on_handle(Packet* packet)
{
	switch (packet->op)
	{
		case I_PING:
			// Just ping.
			break; //
		case I_OFFLINE:
			_LogDebug_("RealmServer", "I_OFFLINE");
			{
				RealmMgr::get_singleton().remove_world_server(this);
			}
			break;
		case I_NOTIFY:
			_LogDebug_("RealmServer", "I_NOTIFY");
			{
				if (packet->param.Long != 2)
				{
					LogError("RealmServer", "Error server are connecting, type : %d", packet->param.Long);
					break;
				}
				RealmMgr::get_singleton().add_world_server(this);
				Packet p;
				p.op = IS_GET_NAME;
				p.len = sizeof(Packet);
				this->send_packet(&p);

				p.op = IS_GET_INFO;
				this->send_packet(&p);

				p.op = IS_GET_STATUS;
				this->send_packet(&p);

				p.op = IS_GET_TYPE;
				this->send_packet(&p);
			}
			break;
		case IS_GET_NAME:
			break;
		case IC_NAME:
			_LogDebug_("RealmServer", "IC_NAME");
			{
				int size = PacketParser::get_ex_len(*packet);
				if (size > 0)
				{
					char* sz = new char[size];
					::memset(sz, 0, size);
					::memcpy(sz, packet->param.Data, size);
					RealmMgr::get_singleton().update_world_server_name(this, std::string(sz));
					delete[] sz;
				}
			}
			break;
		case IS_GET_STATUS:
			break;
		case IC_STATUS:
		case IC_POST_STATUS:
			_LogDebug_("RealmServer", "IC_STATUS | IC_POST_STATUS");
			{
				WorldSrvStatus status = (WorldSrvStatus)packet->param.Long;
				RealmMgr::get_singleton().update_world_server_status(this, status);
			}
			break;
		case IS_GET_INFO:
			break;
		case IC_INFO:
			_LogDebug_("RealmServer", "IC_INFO");
			{
				int size = PacketParser::get_ex_len(*packet);
				if (size > 0)
				{
					RealmMgr::get_singleton().update_world_server_info(this, std::string(packet->param.Data));
				}
			}
			break;
		case IS_GET_TYPE:
			break;
		case IC_TYPE:
			_LogDebug_("RealmServer", "IC_TYPE");
			{
				int size = PacketParser::get_ex_len(*packet);
				WorldSrvType type = (WorldSrvType)packet->param.Long;
				RealmMgr::get_singleton().update_world_server_type(this, type);
			}
		default:
			break;
	}

}