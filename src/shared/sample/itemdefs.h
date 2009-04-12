#ifndef _ITEM_DEFS_H_
#define _ITEM_DEFS_H_

#include "iteminfo.h"
#include "ro_defs.h"
#include <cstring>

namespace ro
{
struct RoCharItem : public srdgame::ItemInfo
{
	RoCharItem() : _amount(0), _identify(0), _refine(0), _attrs(0)
	{
		memset(_cards, 0, sizeof(short) * MaxSlotCount);
	}
	//int _id; // The unique id?
	//short _type; // The item type id // the name_id;
	short _amount; // Mount?
	uint16 _equip; // load from DB // location(s) where item is equipped (using enum equip_pos for bitmasking)
	char _identify; // for job? // Load from DB.
	char _refine; // Load from DDB.
	char _attrs; // Load from DB
	//char _item_type; // The item type. refer to clif.c:1867
	short _cards[MaxSlotCount];
	//unsigned int _expire_time;
};
struct RoDBItem
{
	RoDBItem() :_id(0), _buy_val(0), _sell_val(0), _equip_point(0), _weight(0), _look(0),  _max_chance(0), _atk(0), _def(0), _range(0), _slots(0), _elv(0), _wlv(0), _equip_jobs(0), _equip_upper(0), _equip_sex(0)
	{
		memset(_name, 0, sizeof(char) * MAX_ITEM_NAME_LEN);
		memset(&_flag, 0, sizeof(_flag));
	}
	uint16 _id; // The name_id;
	char _name[MAX_ITEM_NAME_LEN];
	// More
	int _buy_val;
	int _sell_val;
	char _item_type;
	short _equip_point;
	int _weight;
	int _look;
	int _max_chance;
	int _atk;
	int _def;
	int _range;
	int _slots;
	int _elv; // equip_level
	int _wlv; // weapon_level
	unsigned int _equip_jobs;
	unsigned int _equip_upper;
	int _equip_sex;
	struct {
		short no_equip;
		unsigned available : 1;
		unsigned value_notdc : 1;
		unsigned value_notoc : 1;
		unsigned no_refine : 1;
		unsigned delay_consume : 1;
		unsigned trade_restriction : 1;
		unsigned autoequip : 1;
	} _flag;
};

}

#endif
