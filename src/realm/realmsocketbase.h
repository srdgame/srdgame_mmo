#ifndef REALM_SOCKET_BASE_H_
#define REALM_SOCKET_BASE_H_

#include "network.h"
#include "packetdefs.h"
#include "fastqueue.h"


namespace srdgame
{
class RealmWorker;
struct Packet;

// Handle the communication with game clients.
class RealmSocketBase : public TcpSocket
{
	friend class RealmWorker;
public:
	RealmSocketBase();
	virtual ~RealmSocketBase();

protected:
	virtual void on_rev();
	// Worker will call this to response one packet.
	virtual void on_handle(Packet* packet) = 0;
	virtual bool send_packet(Packet* packet);
	void start_worker();
protected:
	FastQueue<Packet> _packets; //FastQueue is thread safe, we no long need to have mutex for it.
	RealmWorker* _worker; // we do not need free this worker, the pool will do for us. to Make sure this will be clear before worker get closed.
	Mutex _worker_lock;
	bool _inter;
	bool _dump_in;
	bool _dump_out;
};
}

#endif
