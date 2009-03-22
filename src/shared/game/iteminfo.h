#ifndef ITEM_INFO_H_
#define ITEM_INFO_H_

namespace srdgame
{
/*
enum ItemType
{
	IT_HEALING = 0,
	IT_UNKNOWN,
	IT_USABLE,
	IT_ETC,
	IT_WEAPON,
	IT_ARMOR,
	IT_CARD,
	IT_PETEGG,
	IT_PETARMOR,
	IT_UNKNOWN2,
	IT_AMMO,
	IT_DELAYCONSUME,
	IT_MAX,
};*/
struct ItemInfo
{
	ItemInfo() : _id(0), _type(0), _expire_time(0)
	{
	}
	int _id;
	int _type;
	int _expire_time;
};

template <class T>
struct ItemInfoEx : public ItemInfo
{
	T _ex;
};

}

#endif
