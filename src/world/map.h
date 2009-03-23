#ifndef MAP_H_
#define MAP_H_

#include <map>
#include <string>
#include "mutex.h"

namespace srdgame
{
class Player;
class Npc;
class Map
{
public:
	Map(const std::string& name);
	virtual ~Map();

public:
	inline std::string get_name(){ return _name;};
	bool add_player(Player* p);
	bool remove_player(Player* p);

	void add_npc(Npc* npc);
	void remove_npc(Npc* npc);
private:
	std::string _name;
	std::map<int, Player*> _players;
	Mutex _players_lock;

	std::map<int, Npc*> _npcs;
	Mutex _npcs_lock;
};
}

#endif
