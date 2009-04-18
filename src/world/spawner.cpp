#include "spawner.h"
#include "configfile.h"
#include "mapmgr.h"
#include "map.h"
#include "npc.h"
#include "mob.h"
#include <fstream>
#include "log.h"

#define NPC_TYPE "NPC"
#define MOB_TYPE "MOB"
#define DOOR_TYPE "DOOR"

#define SP_NAME "name"
#define SP_TYPE "type"
#define SP_ID "id"
#define SP_MAP "map"
#define SP_POS_X "pos_x"
#define SP_POS_Y "pos_y"
#define SP_TIME "time"
#define SP_COUNT "count"
#define SP_SCRIPT "script"

using namespace srdgame;
using namespace std;

Spawner::Spawner(int id) : RoUnit(id)
{
	//
}

Spawner::Spawner(int id, const SpawnerInfo& info) : RoUnit(id), _info(info)
{
}

Spawner::~Spawner()
{
	for (size_t i = 0; i < _npcs.size(); ++i)
	{
		delete _npcs[i];
	}
	_npcs.clear();
}
long Spawner::notify(long time)
{
	if (_npcs.size() >= _info._count)
		return 0;

	Npc * one = NULL;
	switch (_info._type)
	{
		case ST_NPC:
			one = new Npc(this);
			break;
		case ST_MOB:
			one = new Mob(this);
			break;
		case ST_DOOR:
			break;
		default:
			break;
	}
	if (!one)
		return 0;
	_npcs.push_back(one);
	return 0;
}

bool Spawner::save(const std::string& fn)
{
	ofstream file(fn.c_str());
	if (file.bad())
		return false;

	file << SP_NAME << " = " << _info._name << endl;
	switch (_info._type)
	{
		case ST_NPC:
			file << SP_TYPE << " = " << NPC_TYPE << endl;
			break;
		case ST_MOB:
			file << SP_TYPE << " = " << MOB_TYPE << endl;
			break;
		case ST_DOOR:
			file << SP_TYPE << " = " << DOOR_TYPE << endl;
			break;
		default:
			break;
	}
	file << SP_ID << " = " << _info._id << endl;
	file << SP_MAP << " = " << _info._map << endl;
	file << SP_POS_X << " = " << _info._pos_x << endl;
	file << SP_POS_Y << " = " << _info._pos_y << endl;
	file << SP_TIME << " = " << _info._time << endl;
	file << SP_COUNT << " = " << _info._count << endl;
	file << SP_SCRIPT << " = " << _info._script << endl;
	return true;
}
bool Spawner::load(const std::string& fn)
{
	ConfigFile file(fn.c_str());
	if (file.get_value<string>(SP_TYPE) == NPC_TYPE)
	{
		_info._type = ST_NPC;
	}
	else if (file.get_value<string>(SP_TYPE) == MOB_TYPE)
	{
		_info._type = ST_MOB;
	}
	else if (file.get_value<string>(SP_TYPE) == DOOR_TYPE)
	{
		_info._type = ST_DOOR;
	}
	else
	{
		LogError("Spawner", "Unknow spawner type found : %s", file.get_value<char*>(SP_TYPE));
		return false;
	}
	_info._name = file.get_value<string>(SP_NAME);
	_info._id = file.get_value<int>(SP_ID);
	_info._map = file.get_value<string>(SP_MAP);
	_info._pos_x = file.get_value<int>(SP_POS_X);
	_info._pos_y = file.get_value<int>(SP_POS_Y);
	_info._time = file.get_value<long>(SP_TIME);
	_info._count = file.get_value<unsigned int>(SP_COUNT);
	_info._script = file.get_value<string>(SP_SCRIPT);
	LogDebug("Spawner", "Finished to load one spawner[ name:%s\tid:%d\tmap:%s]", _info._name.c_str(), _info._id, _info._map.c_str());
	return true;
}


