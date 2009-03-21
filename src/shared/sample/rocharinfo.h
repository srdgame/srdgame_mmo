#ifndef RO_CHAR_INFO_H_
#define RO_CHAR_INFO_H_

#include "metadefs.h"
#include "charinfo.h"

using namespace srdgame;

namespace ro;
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
	uint32 _id; // The unique id?
	uint32 _type; // The item type
	uint32 _amount; // Mount?
	uint32 _identify; // for job?
	uint32 _refine; 
	uint32 _attrs;
	RoCharItemCard _cards[MaxItemSlotCount];
	uint32 _expire_time;
};
struct RoCharSkill : public Skill
{

};
struct RoCharFamily : public Family
{

};
enum RoCharMajorType
{
	MT_ARCH = 0,
	MT_SPEAR,
	MT_SWORD,
	MT_COUNT;
};
struct RoCharMajor
{
	RoCharMajorType _type;
	uint32 _faith;
	uint32 _calls;
};
struct RoCharFriend : public Friend
{

};
struct RoCharHotKey
{
	uint32 id;
	uint16 lvl;
	uint16 type; // 0: item, 1:skill
};
class RoCharInfo : public CharInfo
{
protected:
	RoCharFamily _family;
	RoCharProp _prop;
	RoCharFace _face;
	RoCharExp _exp;
	RoCharItem _items[MaxItemCount];
	RoCharSkill _skills[MaxSkillCount];
	RoCharFriend _friends[MaxFriendCount];
	RoCharHotKey _hotkeys[MaxHotKeyCount]
	// More:
	uint32 _option;
	uint32 _party_id;
	uint32 _guild_id;
	uint32 _pet_id;
	uint32 _home_id;
	uint32 _mer_id; // ????
	
	uint32 _fame; //声望

	RoCharMajor _majors[MT_COUNT];

	uint32 _weapon;
	uint32 _shield;

	uint32 _hungry;
};
}
#endif
