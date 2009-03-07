#include "loginsocketr.h"
#include "log.h"
#include "packetparser.h"
#include "loginworker.h"

using namespace srdgame;

LoginInterSocketR::LoginInterSocketR()
	: LoginSocketBase()
{
}

LoginInterSocketR::~LoginInterSocketR()
{

}

void LoginInterSocketR::on_rev()
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
		LogSuccess("LoginServer", "Comming Data: %s", new_data);
		delete[] new_data;*/
		size_t index = 0;
		while (true)
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
			LogDebug("LoginInterSocketR", "One packet received");
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

void LoginInterSocketR::on_send()
{
}

void LoginInterSocketR::on_connect()
{
}

void LoginInterSocketR::on_close()
{
}

