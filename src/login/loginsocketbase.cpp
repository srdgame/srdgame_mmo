#include "loginworker.h"
#include "packetdefs.h"
#include "loginsocketbase.h"
#include "packetparser.h"
#include "threadpool.h"
#include "autolock.h"
#include "packetdump.h"

using namespace srdgame;

#define SOCKET_DEBUG
#undef _LogDebug_

#ifdef SOCKET_DEBUG
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

LoginSocketBase::LoginSocketBase() : _worker(NULL), _dump_in(false), _dump_out(false), _inter(true)
{
}
LoginSocketBase::~LoginSocketBase()
{
	_worker_lock.lock();
	if (_worker)
	{
		_worker->shutdown();
	}
	_worker_lock.unlock();
}

bool LoginSocketBase::send_packet(Packet* packet)
{
	_LogDebug_("LoginServer", "Starting to send packet, opcode: %d", packet->op);

	// TODO: To refine this send, the buffer allocation is useless here, we could use send_buf here.
	char sz[MAX_PACKET_LEN];
	::memset(sz, 0, MAX_PACKET_LEN);
	size_t size = 0;
	
	if (_inter)
	{
		size = PacketParser::get_singleton().to_inter(sz, *packet);
	}
	else
	{
		size = PacketParser::get_singleton().to_ex(sz, *packet);
	}

	if (size && _dump_out)
	{
		PacketDump::get_singleton().dump("PACKET OUT", sz, size);
		PacketDump::get_singleton().dump("PACKET OUT", *packet);
	}

	_LogDebug_("LoginServer", "Packet length is : %d", size);
	
	if (size && is_connected())
	{
		return this->send(sz, size);
	}

	// We allow parser ingore this packet.
	if (size == 0)
		return true;

	LogError("LoginServer", "Socket is't been connected");
	
	return false;
}
void LoginSocketBase::on_rev()
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
			size_t used = _inter ? PacketParser::get_singleton().from_inter(p, data + index, size - index)
				: PacketParser::get_singleton().from_ex(p, data + index, size - index);
			if (!used)
			{
				if (size - index >= MAX_PACKET_LEN)
				{
					// we should quit here.close connection.
					this->close();
				}
				if (_dump_in)
				{
					PacketDump::get_singleton().dump("UNCOMPLETE PACKET", data + index, size - index);
					//PacketDump::get_singleton().dump("UNKNOWN PACKET", p);
				}
				LogDebug("LoginSocket", "Breaking!!! size is : %d   index is : %d", size, index);
				break;
			}
			LogDebug("LoginSocket", "One packet received from world server");
			// For dump 
			if (_dump_in)
			{
				PacketDump::get_singleton().dump("PACKET", data + index, used);
				PacketDump::get_singleton().dump("PACKET", p);
			}

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

void LoginSocketBase::start_worker()
{
	AutoLock lock(_worker_lock);
	if (_worker)
		return;

	LogDebug("LoginServer", "Starting worker thread");
	_worker = new LoginWorker(this);
	ThreadPool::get_singleton().execute(_worker);
}
