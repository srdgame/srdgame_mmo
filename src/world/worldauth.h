#ifndef WORLD_AUTH_H_
#define WORLD_AUTH_H_

#include "singleton.h"

namespace ro
{
struct ConnectToMap;
}

namespace srdgame
{
class WorldSocket;
class Packet;
class Player;
class WorldAuth : public Singleton < WorldAuth >
{
public:
	WorldAuth();
	~WorldAuth();

	void handle_login(WorldSocket* socket, const Packet* packet);
	void handle_admin_login(WorldSocket* socket, const Packet* packet);


protected:
	bool auth_char(WorldSocket* socket);
	void reg_to_map(Player* p);
};
}

#endif
