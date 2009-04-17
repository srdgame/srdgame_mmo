#include "npc.h"
#include "npcid.h"
#include "spawner.h"
#include "npcobject.h"
#include "mapmgr.h"
#include "map.h"

using namespace srdgame;

Npc::Npc(Spawner* sp) : _sp (sp), _obj(NULL)
{
	_id = NpcId::generate(); // generate the id;
	_info._id = sp->get_info()._id;

	Map* map = MapMgr::get_singleton().get_map(_sp->get_info()._map);
	if (map)
	{
		NpcObject* obj = new NpcObject(*this);
		map->add_unit(obj);
	}
}

Npc::~Npc()
{
	if (_obj)
	{

		Map* map = MapMgr::get_singleton().get_map(_sp->get_info()._map);
		if (map)
		{
			map->remove_unit(_obj);
		}
		delete _obj;
	}
}


