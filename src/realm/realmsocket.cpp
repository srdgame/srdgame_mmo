#include "realmsocket.h"
#include "log.h"
#include "packetparser.h"
#include "realmworker.h"

using namespace srdgame;

RealmSocket::RealmSocket(bool inter)
	: TcpSocket(/* using the default buffer size */)
	  , _inter(inter)
	  , _worker(NULL)
{
}

RealmSocket::~RealmSocket()
{
	if (_worker && _worker->is_running())
	{
		_worker->shutdown();
	}
}

void RealmSocket::on_rev()
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
		LogSuccess("RealmServer", "Comming Data: %s", new_data);
		delete[] new_data;*/
		size_t index = 0;
		while (true)
		{
			Packet p;
			size_t used = 0;
			if (_inter)
			{
				used = PacketParser::get_singleton().from_inter(p, data + index, size - index);
			}
			else
			{
				used = PacketParser::get_singleton().from_ex(p, data + index, size - index);
			}
			if (!used)
			{
				if (size - index >= MAX_PACKET_LEN)
				{
					// we should quit here.close connection.
					this->close();
				}
				break;
			}
			LogDebug("RealmSocket", "One packet received");
			index += used;
			_packets.push(p);
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

void RealmSocket::on_send()
{
}

void RealmSocket::on_connect()
{
}

void RealmSocket::on_close()
{
}

RealmSocketInter::RealmSocketInter() : RealmSocket(true)
{
	LogDebug("RealmServer", "Internal socket connection coming");
	if (!_inter)
	{
		LogError("RealmServer", "You are failed to create internal socket");
	}
}
