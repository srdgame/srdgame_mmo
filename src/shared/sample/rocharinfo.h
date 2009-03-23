#ifndef RO_CHAR_INFO_H_
#define RO_CHAR_INFO_H_

#include "metadefs.h"
#include "gamedefs.h"
#include "ro_defs.h"
#include <cstring>

using namespace srdgame;

namespace ro
{
static const uint16 MaxItemCount = 10; // TODO:
static const uint8 MaxItemSlotCount = 16;
static const uint16 MaxSkillCount = 255;
static const uint16 MaxFriendCount = 255;
static const uint16 MaxHotKeyCount = 255;
struct RoCharProp : public NpcProp
{

};
struct RoCharShow : public NpcShow
{

};
struct RoCharExp : public CharExp
{

};
struct RoCharItemCard
{
};
struct RoCharItem
{
	RoCharItem() : _id(0), _type(0), _amount(0), _identify(0), _refine(0), _attrs(0), _expire_time(0)
	{
	}
	int _id; // The unique id?
	int _type; // The item type
	int _amount; // Mount?
	int _identify; // for job?
	int _refine; 
	int _attrs;
	RoCharItemCard _cards[MaxItemSlotCount];
	int _expire_time;
};
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
class RoCharInfo : public CharInfo
{
public:
	RoCharInfo() : _option(0), _party_id(0), _guild_id(0), _pet_id(0), _home_id(0), _mer_id(0), _homun_id(0), _fame(0), _slot(0), _hungry(0)
	{
	}
public:
	RoCharFamilyInfo _family;
	RoCharProp _prop;
	RoCharShow _show;
	RoCharExp _exp;
	RoCharItem _items[MaxItemCount];
	RoCharSkillInfo _skills[MaxSkillCount];
	RoCharFriendInfo _friends[MaxFriendCount];
	RoCharHotKey _hotkeys[MaxHotKeyCount];
	// More:
	int _option;
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
};
}
#endif
