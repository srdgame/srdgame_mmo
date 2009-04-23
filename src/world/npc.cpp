#include "npc.h"
#include "npcid.h"
#include "spawner.h"
#include "mapmgr.h"
#include "map.h"
#include "ro_defs.h"
#include "log.h"

using namespace srdgame;

Npc::Npc(Spawner* sp) : _sp (sp), _unit(NULL)
{
	LogDebug("NPC", "Creating NPC..................");
	_info._id = NpcId::generate(); // generate the id;
	LogDebug("NPC", "Get NPC id : %d ", _info._id);

	const SpawnerInfo& info = _sp->get_spawn_info();
	_info._class = info._id;
	_info._show._class = info._id;
	_info._name = info._name;
	_info._last_pos._x = info._pos_x;
	_info._last_pos._y = info._pos_y;
	_info._exp._base_lvl = 99;
	_info._exp._job_lvl = 99;
	_info._status._speed = 200;

	Map* map = MapMgr::get_singleton().get_map(info._map);
	if (map)
	{
		LogDebug("NPC", "Adding npc : %d to map", _info._id);
		_unit = new RoUnit(_info._id, UT_NPC);
		_unit->set_info(&_info);
		map->add_unit(_unit);
	}
}

Npc::~Npc()
{
	if (_unit)
	{
		LogError("NPC", "Removing NPC............");
		Map* map = _unit->get_map();
		if (map)
		{
			map->remove_unit(_unit);
		}
		delete _unit;
	}
}


