#include "loginworker.h"
#include "packetdefs.h"
#include "loginsocketbase.h"
#include "packetparser.h"
#include "threadpool.h"
#include "autolock.h"

using namespace srdgame;

#define SOCKET_DEBUG

LoginSocketBase::LoginSocketBase() : _worker(NULL)
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
#ifdef SOCKET_DEBUG
	LogDebug("LoginServer", "Starting to send packet, opcode: %d", packet->op);
#endif
	// TODO: To refine this send, the buffer allocation is useless here, we could use send_buf here.
	char sz[MAX_PACKET_LEN];
	::memset(sz, 0, MAX_PACKET_LEN);
	size_t size = PacketParser::get_singleton().to_inter(sz, *packet);

#ifdef SOCKET_DEBUG
	LogDebug("LoginServer", "Packet length is : %d", size);
#endif
	if (size && is_connected())
	{
		return this->send(sz, size);
	}
#ifdef SOCKET_DEBUG
	LogError("LoginServer", "Socket is't been connected");
#endif
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

void LoginSocketBase::start_worker()
{
	AutoLock lock(_worker_lock);
	if (_worker)
		return;

	LogDebug("LoginServer", "Starting worker thread");
	_worker = new LoginWorker(this);
	ThreadPool::get_singleton().execute(_worker);
}
