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
class ItemInfo
{
public:
	ItemInfo(uint32 id, uint32 type) : _id(id), _type(type){};
	virtual ~ItemInfo(){}

	inline uint32 get_id()
	{
		return _id;
	}
	inline uint32 get_type()
	{
		return _type;
	}
protected:
	uint32 _id;
	uint32 _type;
	uint32 _expire_time;
};

template <class T>
class ItemInfoEx : public ItemInfo
{
public:
	ItemInfoEx(uint32 type, T& t) : ItemInfo(type), _ex(t)
	{
	}
	virtual ~ItemInfoEx()
	{
	}

	T* get_ex_data()
	{
		return &_ex;
	}
protected:
	T _ex;
}

}

#endif
