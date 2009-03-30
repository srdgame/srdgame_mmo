#ifndef PLAYER_H_
#define PLAYER_H_
#include <string>

//#include "charinfo.h"

namespace srdgame
{
struct CharInfo;
class Map;
class WorldSocket;
class Position;
class Player
{
public:
	Player(WorldSocket* socket, int acc_id, int id);
	virtual ~Player();
	virtual CharInfo* get_info(); // warry about this.

	virtual std::string get_last_map();
	virtual bool set_map(Map* map);

	virtual int get_id();
	virtual int get_acc_id();

	virtual WorldSocket* get_socket();
protected:
	WorldSocket* _socket;
	int _acc_id;
	int _id;
	CharInfo* _info;
	Map* _map;
};
}

#endif
