#ifndef LOGIN_INTER_SOCKET_R_H_
#define LOGIN_INTER_SOCKET_R_H_

#include "worldsocketbase.h"
#include "packetdefs.h"


namespace srdgame
{
class WorldWorker;
struct Packet;
class WorldServer;

// Handle the communication with world server.
class InterSocket : public WorldSocketBase
{
	friend class WorldWorker;
	friend class WorldServer;
public:
	InterSocket(WorldServer* server);
	virtual ~InterSocket();
	virtual void on_rev();
	virtual void on_send();
	virtual void on_connect();
	virtual void on_close();

public:
	// Worker will call this to response one packet.
	virtual void on_handle(Packet* packet);
protected:
	WorldServer* _server;
};


}

#endif
