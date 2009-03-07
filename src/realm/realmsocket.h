#ifndef REALM_SOCKET_H_
#define REALM_SOCKET_H_

#include "network.h"
#include "packetdefs.h"
#include "fastqueue.h"


namespace srdgame
{
class RealmWorker;
class RealmSocket : public TcpSocket
{
	friend class RealmWorker;
public:
	RealmSocket(bool inter = false);
	virtual ~RealmSocket();
	virtual void on_rev();
	virtual void on_send();
	virtual void on_connect();
	virtual void on_close();

protected:
	bool _inter;
	FastQueue<Packet> _packets; //FastQueue is thread safe, we no long need to have mutex for it.
	RealmWorker* _worker; // we do not need free this worker, the pool will do for us. to Make sure this will be clear before worker get closed.
};

class RealmSocketInter : public RealmSocket
{
	friend class RealmWorker;
public:
	RealmSocketInter();	
};

}

#endif
