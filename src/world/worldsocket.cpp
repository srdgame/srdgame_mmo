#include "worldsocket.h"
#include "log.h"
#include "packetparser.h"
#include "worldworker.h"
#include "packetdump.h"
#include "opcode.h"
#include "ro_defs.h"
#include "worldauth.h"

using namespace srdgame;
using namespace srdgame::opcode;
using namespace ro;
#undef _LogDebug_
#define _LogDebug_ LogDebug

WorldSocket::WorldSocket()
	: WorldSocketBase(/* using the default buffer size */)
{
}

WorldSocket::~WorldSocket()
{
	if (_worker && _worker->is_running())
	{
		_worker->shutdown();
	}
}

void WorldSocket::on_rev()
{
	BufferBase* buf = get_rev_buf();
	size_t size;
	char* data = buf->get_data(size);
	if (size != 0)
	{
		/*char* new_data = new char[size + 1];
		::memset(new_data, 0, size+1);
		::memcpy(new_data, data, size);
		LogSuccess("WorldServer", "Comming Data: %s", new_data);
		delete[] new_data;*/
		size_t index = 0;
		while (size > index)
		{
			Packet p;
			size_t used = PacketParser::get_singleton().from_ex(p, data + index, size - index);
			if (!used)
			{
				if (size - index >= MAX_PACKET_LEN)
				{
					// we should quit here.close connection.
					this->close();
				}
				// Ok, we have to do somthing here.
				if (buf->data_size() > size && size != 0)
				{
				//	buf->arrange();
					_LogDebug_("RealmSocket", "Arranging~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
					char b_data[MAX_PACKET_LEN];
					size_t o_size = size - index;
					memset(b_data, 0, MAX_PACKET_LEN);
					memcpy(b_data, data + index, size - index);
					index = 0;
					buf->free(size);
					data = buf->get_data(size);
					memcpy(b_data + o_size, data, min(size, MAX_PACKET_LEN - o_size));
					_LogDebug_("RealmSocket", "o_size : %d, size : %d", o_size, size);
					_LogDebug_("RealmSocket", "converting bytes : %d", min((size_t)MAX_PACKET_LEN, size + o_size));
					size_t used_i = _inter ? PacketParser::get_singleton().from_inter(p, b_data , min((size_t)MAX_PACKET_LEN, size + o_size))
									: PacketParser::get_singleton().from_ex(p, b_data , min((size_t)MAX_PACKET_LEN, size + o_size));
					if (!used_i)
					{
						this->close();
					}
					else
					{
						// 
						_LogDebug_("RealmSocket", "We are good to have this to avoid ***");
						index = used_i - o_size;
						buf->free(index);
						continue;
					}
				}
				break;
			}
			LogDebug("WorldSocket", "One packet received");
			PacketDump::get_singleton().dump("PACKET", data + index, used);
			PacketDump::get_singleton().dump("PACKET", p);

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

void WorldSocket::on_send()
{
}

void WorldSocket::on_connect()
{
}

void WorldSocket::on_close()
{
}

void WorldSocket::on_handle(Packet* packet)
{
	switch (packet->op)
	{
		case EC_CONNECT_TO_MAP:
			{
				WorldAuth::get_singleton().handle_login(this, packet);	
			}
			break;
		default:
			break;
	}
}

