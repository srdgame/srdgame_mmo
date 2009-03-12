#include "worldsocket.h"
#include "log.h"
#include "packetparser.h"
#include "worldworker.h"

using namespace srdgame;

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
	lock_rev_buf();
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
				break;
			}
			LogDebug("WorldSocket", "One packet received");
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
	unlock_rev_buf();
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
		default:
			break;
	}
}

