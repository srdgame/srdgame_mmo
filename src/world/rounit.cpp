#include "rounit.h"
#include "player.h"
#include "packetdefs.h"
#include "opcode.h"
#include "ro_defs.h"
#include "log.h"

using namespace ro;
using namespace srdgame;
using namespace srdgame::opcode;

RoUnit::RoUnit(int id, UnitType type) : Unit(id), _type(type), _info(NULL), _pri(NP_NOTHING), _time(0), _by(NULL), _map(NULL)
{
}

RoUnit::~RoUnit()
{
}

long RoUnit::notify(long time)
{
	_time = time;
	return _time + _pri * 1000;
}

bool RoUnit::move_to(Position* pos, Object* by)
{
	_pos._x = pos->_x;
	_pos._y = pos->_y;
	_pos._z = pos->_z;
	_pos._r = pos->_r;
	_by = by;
	return true;
}

bool RoUnit::clicked(Object* by)
{
	_by = by;
	
	return true;
}

bool RoUnit::attacked(Object* by)
{
	_by = by;
	return true;
}

void RoUnit::get_names(Player* p)
{	
	
	// TODO: test whether we could only use the ex name.
	if (_type = UT_NPC)
	{
		// For npc name.
		Packet packet(ES_REQUEST_CHAR_NAME);
		RoRequestCharName name;
		memset(&name, 0, sizeof(name));
		name._id = this->get_id();
		memcpy(name._name, _info->_name.c_str(), MAX_NAME_LEN);
		packet.len = sizeof(Packet) + sizeof(name);
		packet.param.Data = (char*) &name;
		p->send_packet(&packet);
	}
	else
	{
		Packet packet(ES_REQUEST_CHAR_NAME_EX);
		RoRequestCharNameEx name;
		memset(&name, 0, sizeof(name));
		name._id = this->get_id();
		memcpy(name._name, _info->_name.c_str(), MAX_NAME_LEN);
		packet.len =sizeof(packet) + sizeof(name);
		packet.param.Data = (char*)&name;
		p->send_packet(&packet);
	}

}

void RoUnit::send_info(Player* p)
{
	if (!_info)
	{
		LogError("MAP", "Unit has no info");
		return;
	}

	Packet packet(ES_MOB_INFO);
	packet.len = sizeof(Packet) + sizeof(RoCharInfoBase);

	packet.param.Data = (char*) _info;
	p->send_packet(&packet);

	LogDebug("MAP", "Unit in X:%d \t Y:%d \t TYPE: %d", _info->_last_pos._x, _info->_last_pos._y, _info->_show._class);
}

void RoUnit::get_item_value(Player* p)
{
	// Check all the items player has, and provide the values of his/her items.
	//TODO:	
}
void RoUnit::get_selling_list(Player* p)
{
	// Send out the items we are selling.
	RoSellingItem items[10];
	memset(items, 0, sizeof(RoSellingItem) * 10);
	items[0]._val = 10;
	items[0]._s_val = 0;
	items[0]._item_type = 4;
	items[0]._item_id = 1201;
	RoSellingItemList list;
	list._count = 1;
	list._items = items;
	
	Packet selling(ES_NPC_SELLING_LIST);
	selling.len = sizeof(Packet) + sizeof(list);
	selling.param.Data = (char*) &list;

	p->send_packet(&selling);

}
