#include "loginsocketw.h"
#include "log.h"
#include "packetparser.h"
#include "loginworker.h"
#include "loginmgr.h"

using namespace srdgame;

LoginInterSocketW::LoginInterSocketW()
	: LoginSocketBase()
{
}

LoginInterSocketW::~LoginInterSocketW()
{
	
}

void LoginInterSocketW::on_rev()
{
	BufferBase* buf = get_rev_buf();
	size_t size;
	char* data = buf->get_data(size);
	if (size != 0)
	{
		/*char* new_data = new char[size + 1];
		::memset(new_data, 0, size+1);
		::memcpy(new_data, data, size);
		LogSuccess("LoginServer", "Comming Data: %s", new_data);
		delete[] new_data;*/
		size_t index = 0;
		while (size > index)
		{
			Packet p;
			size_t used = PacketParser::get_singleton().from_inter(p, data + index, size - index);
			if (!used)
			{
				if (size - index >= MAX_PACKET_LEN)
				{
					// we should quit here.close connection.
					this->close();
				}
				break;
			}
			LogDebug("LoginInterSocketW", "One packet received from world server");
			index += used;
			_packets.push(p);
			start_worker();
		}
		buf->free(index);

		// Ask for worker.
	}
	else
	{
		buf->free(size);
	}
}

void LoginInterSocketW::on_send()
{
}

void LoginInterSocketW::on_connect()
{
}

void LoginInterSocketW::on_close()
{
	LogDebug("LoginServer", "connection with world server has been dropdown");
}
void LoginInterSocketW::on_handle(Packet* packet)
{
	switch (packet->op)
	{
		case I_PING:
			// Just ping.
			break; //
		case I_OFFLINE:
			_LogDebug_("LoginServer", "I_OFFLINE");
			{
				LoginMgr::get_singleton().remove_world_server(this);
			}
			break;
		case I_NOTIFY:
			_LogDebug_("LoginServer", "I_NOTIFY");
			{
				if (packet->param.Long != 2)
				{
					LogError("LoginServer", "Error server are connecting, type : %d", packet->param.Long);
					break;
				}
				LoginMgr::get_singleton().add_world_server(this);
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
			_LogDebug_("LoginServer", "IC_NAME");
			{
				int size = PacketParser::get_ex_len(*packet);
				if (size > 0)
				{
					char* sz = new char[size];
					::memset(sz, 0, size);
					::memcpy(sz, packet->param.Data, size);
					LoginMgr::get_singleton().update_world_server_name(this, std::string(sz));
					delete[] sz;
				}
			}
			break;
		case IS_GET_STATUS:
			break;
		case IC_STATUS:
		case IC_POST_STATUS:
			_LogDebug_("LoginServer", "IC_STATUS | IC_POST_STATUS");
			{
				WorldSrvStatus status = (WorldSrvStatus)packet->param.Long;
				LoginMgr::get_singleton().update_world_server_status(this, status);
			}
			break;
		case IS_GET_INFO:
			break;
		case IC_INFO:
			_LogDebug_("LoginServer", "IC_INFO");
			{
				int size = PacketParser::get_ex_len(*packet);
				if (size > 0)
				{
					LoginMgr::get_singleton().update_world_server_info(this, std::string(packet->param.Data));
				}
			}
			break;
		case IS_GET_TYPE:
			break;
		case IC_TYPE:
			_LogDebug_("LoginServer", "IC_TYPE");
			{
				int size = PacketParser::get_ex_len(*packet);
				WorldSrvType type = (WorldSrvType)packet->param.Long;
				LoginMgr::get_singleton().update_world_server_type(this, type);
			}
		default:
			break;
	}

}
