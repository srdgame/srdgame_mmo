#ifndef REALM_INTER_SOCKET_W_H_
#define REALM_INTER_SOCKET_W_H_

#include "realmsocketbase.h"
#include "packetdefs.h"
#include "fastqueue.h"


namespace srdgame
{
class RealmWorker;
struct Packet;

// Handle the communication with world server.
class RealmInterSocketW : public RealmSocketBase
{
	friend class RealmWorker;
public:
	RealmInterSocketW();
	virtual ~RealmInterSocketW();
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
