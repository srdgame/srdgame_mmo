#ifndef LOGIN_SOCKET_H_
#define LOGIN_SOCKET_H_

#include "worldsocketbase.h"
#include "packetdefs.h"
#include "mutex.h"

namespace srdgame
{
class WorldWorker;
struct Packet;
class Player;
// Handle the communication with game clients.
class WorldSocket : public WorldSocketBase
{
	friend class WorldWorker;
public:
	WorldSocket();
	virtual ~WorldSocket();
	virtual void on_rev();
	virtual void on_send();
	virtual void on_connect();
	virtual void on_close();

	void bind(Player* p);
public:
	// Worker will call this to response one packet.
	virtual void on_handle(Packet* packet);
protected:
	Player* _player;
	Mutex _lock;
};
}

#endif
