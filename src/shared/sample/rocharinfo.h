#ifndef RO_CHAR_INFO_H_
#define RO_CHAR_INFO_H_

#include "metadefs.h"
#include "gamedefs.h"
#include "ro_defs.h"
#include "rounitstatus.h"
#include "itemdefs.h"
#include "mutex.h"
#include <cstring>
#include <vector>

using namespace srdgame;

namespace ro
{
struct RoCharProp : public NpcProp
{

};
struct RoCharShow : public NpcShow
{
	//int _weapon2;
};
struct RoCharExp : public CharExp
{

};
/*struct RoCharItem : public ItemInfo
{
	RoCharItem() : _amount(0), _identify(0), _refine(0), _attrs(0), _item_type(0)
	{
		memset(_cards, 0, sizeof(short) * MaxSlotCount);
	}
	//int _id; // The unique id?
	//short _type; // The item type id
	short _amount; // Mount?
	uint16 _equip; // load from DB
	char _identify; // for job? // Load from DB.
	char _refine; // Load from DDB.
	char _attrs; // Load from DB
	char _item_type; // The item type. refer to clif.c:1867
	short _cards[MaxSlotCount];
	//unsigned int _expire_time;
};*/ // Moved to itemdefs.h
struct RoCharSkillInfo : public SkillInfo
{

};
struct RoCharFamilyInfo : public NpcFamilyInfo
{

};
enum RoCharMajorType
{
	MT_ARCH = 0,
	MT_SPEAR,
	MT_SWORD,
	MT_COUNT,
};
struct RoCharMajor
{
	RoCharMajor() : _type(MT_ARCH), _faith(0), _calls(0)
	{
	}
	RoCharMajorType _type;
	int _faith;
	int _calls;
};
struct RoCharFriendInfo : public FriendInfo
{

};
struct RoCharHotKey
{
	RoCharHotKey() : _id(0), _lvl(0), _type(0)
	{
	}
	int _id;
	uint16 _lvl;
	uint16 _type; // 0: item, 1:skill
};

struct RoPosition : public Position
{
	RoPosition()
	{
		memset(_map_name, 0, MAX_MAP_NAME_LEN);
	}
	char _map_name[MAX_MAP_NAME_LEN];
};
class RoCharInfoBase : public CharInfo
{
public:
	RoCharInfoBase() : _party_id(0), _guild_id(0), _pet_id(0), _home_id(0), _mer_id(0), _homun_id(0), _fame(0), _slot(0), _hungry(0)
	{
	}
public:
	RoCharFamilyInfo _family;
	RoCharProp _prop;
	RoCharShow _show;
	RoCharExp _exp;
	RoUnitStatus _status;

	// Thos information won't be used by load_chars.
	//RoCharItem _items[MaxItemCount];
	//RoCharSkillInfo _skills[MaxSkillCount];
	//RoCharFriendInfo _friends[MaxFriendCount];
	//RoCharHotKey _hotkeys[MaxHotKeyCount];
	//
	// More:
	// int _option; // moved to _status;
	int _party_id;
	int _guild_id;
	int _pet_id;
	int _home_id;
	int _mer_id; // ????
	int _homun_id;

	
	int _fame; //声望

	RoCharMajor _majors[MT_COUNT];

	int _slot; // The slot for select chars
	int _hungry;

	// Map
	RoPosition _last_pos;
	RoPosition _save_pos;

	// Mutex, for threading
	srdgame::Mutex _lock;
};
class RoCharInfo : public RoCharInfoBase
{
public:
	std::vector<RoCharItem> _items;
	std::vector<RoCharItem> _carts;
	std::vector<RoCharSkillInfo> _skills;
	std::vector<RoCharFriendInfo> _friends;
	std::vector<RoCharHotKey> _hotkeys;
};
}
#endif
