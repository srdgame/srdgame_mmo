#ifndef REALM_INTER_SOCKET_L_H_
#define REALM_INTER_SOCKET_L_H_

#include "realmsocketbase.h"
#include "packetdefs.h"


namespace srdgame
{
class RealmWorker;
struct Packet;
class RealmServer;

// Handle the communication with world server.
class RealmInterSocketL : public RealmSocketBase
{
	friend class RealmWorker;
	friend class RealmServer;
public:
	RealmInterSocketL(RealmServer* server);
	virtual ~RealmInterSocketL();
	//virtual void on_rev();
	virtual void on_send();
	virtual void on_connect();
	virtual void on_close();

public:
	// Worker will call this to response one packet.
	virtual void on_handle(Packet* packet);
protected:
	RealmServer* _server;
};


}

#endif
