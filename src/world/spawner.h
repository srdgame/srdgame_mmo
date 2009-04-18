#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "rounit.h"
#include "rocharinfo.h"

using ro::RoPosition;

#include <string>
#include <vector>

namespace srdgame
{
enum SpawnType
{
	ST_NONE,
	ST_NPC,
	ST_MOB,
	ST_DOOR,
};
struct SpawnerInfo
{
	SpawnerInfo() : _type(ST_NONE), _id(0), _pos_x(0), _pos_y(0), _time(0), _count(0)
	{
	}
	std::string _name;
	SpawnType _type;
	int _id; // mob id or npc type, or..
	std::string _map;
	int _pos_x;
	int _pos_y;
	long _time;
	unsigned int _count;
	std::string _script;
};
class Npc;
class Spawner : public RoUnit
{
	friend class SpawnerMgr;
protected:
	Spawner(int id);
	Spawner(int id, const SpawnerInfo& info);
	~Spawner();

public:
	long notify(long time);

	bool save(const std::string& fn);
	bool load(const std::string& fn);

	inline const SpawnerInfo& get_spawn_info()
	{
		return _info;
	}
	RoCharInfoBase* get_info()
	{
		return NULL;
	}
protected:
	SpawnerInfo _info;
	std::vector<Npc*> _npcs;
};
}

#endif
