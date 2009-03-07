#ifndef LOGIN_SOCKET_BASE_H_
#define LOGIN_SOCKET_BASE_H_

#include "network.h"
#include "packetdefs.h"
#include "fastqueue.h"


namespace srdgame
{
class LoginWorker;
struct Packet;

// Handle the communication with game clients.
class LoginSocketBase : public TcpSocket
{
	friend class LoginWorker;
public:
	LoginSocketBase();
	virtual ~LoginSocketBase();

public:
	// Worker will call this to response one packet.
	virtual bool on_handle(Packet* packet) = 0;
protected:
	FastQueue<Packet> _packets; //FastQueue is thread safe, we no long need to have mutex for it.
	LoginWorker* _worker; // we do not need free this worker, the pool will do for us. to Make sure this will be clear before worker get closed.
	Mutex _worker_lock;
};
}

#endif
