#ifndef LOGIN_SOCKET_BASE_H_
#define LOGIN_SOCKET_BASE_H_

#include "network.h"
#include "packetdefs.h"
#include "fastqueue.h"


namespace srdgame
{
class WorldWorker;
struct Packet;

// Handle the communication with game clients.
class WorldSocketBase : public TcpSocket
{
	friend class WorldWorker;
public:
	WorldSocketBase();
	virtual ~WorldSocketBase();

	virtual bool send_packet(Packet* packet);
protected:
	// Worker will call this to response one packet.
	virtual void on_handle(Packet* packet) = 0;
	void start_worker();
protected:
	FastQueue<Packet> _packets; //FastQueue is thread safe, we no long need to have mutex for it.
	WorldWorker* _worker; // we do not need free this worker, the pool will do for us. to Make sure this will be clear before worker get closed.
	bool _inter;
	Mutex _worker_lock;
};
}

#endif
