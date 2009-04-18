#ifndef MAP_H_
#define MAP_H_

#include <map>
#include <string>
#include "mutex.h"
#include "romap.h"
#include "unitmgr.h"

namespace srdgame
{
class Player;
class Npc;
struct Packet;
// This is one lazy map, it won't construct almost data when there is no user.  And the data will be constructed once one player joins it.
class Map
{
public:
	Map(const std::string& name, int id);
	virtual ~Map();

public:
	void bind(ro::MapData* data);

public:
	inline std::string get_name() const { return _name;};
	inline int get_id() const { return _id;};
	bool add_player(Player* p);
	bool remove_player(Player* p);

	//inline UnitMgr& get_unit_mgr() { return _mgr; }
	void add_unit(RoUnit* unit);
	void remove_unit(RoUnit* unit);

	void send_msg(const std::string& msg, int from_id);

	void request_char_name(Player* p, int id);
protected:
	void send_packet(Packet* p, int from_id = 0, bool skip_self = false);

	void send_all_units(Player* p);
private:
	std::string _name;
	int _id;
	std::map<int, Player*> _players;
	Mutex _players_lock;

	ro::MapData* _ro_map; // read only?
	UnitMgr _mgr;
	Mutex _lock;
};
}

#endif
