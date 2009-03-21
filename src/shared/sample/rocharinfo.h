#ifndef RO_CHAR_INFO_H_
#define RO_CHAR_INFO_H_

#include "metadefs.h"
#include "gamedefs.h"
#include "ro_defs.h"

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
	RoCharMajorType _type;
	int _faith;
	int _calls;
};
struct RoCharFriendInfo : public FriendInfo
{

};
struct RoCharHotKey
{
	int id;
	uint16 lvl;
	uint16 type; // 0: item, 1:skill
};
struct RoPosition : public Position
{
	char _name[MAX_MAP_NAME_LEN];
};
class RoCharInfo : public CharInfo
{
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
