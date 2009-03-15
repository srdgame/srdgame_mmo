#ifndef REALM_SOCKET_H_
#define REALM_SOCKET_H_

#include "realmsocketbase.h"
#include "packetdefs.h"


namespace srdgame
{
class RealmWorker;
struct Packet;

// Handle the communication with game clients.
class RealmSocket : public RealmSocketBase
{
	friend class RealmWorker;
	friend class RealmAuth;
public:
	RealmSocket();
	virtual ~RealmSocket();
	//virtual void on_rev();
	virtual void on_send();
	virtual void on_connect();
	virtual void on_close();

public:
	// Worker will call this to response one packet.
	virtual void on_handle(Packet* packet);

protected:
};
}

#endif
