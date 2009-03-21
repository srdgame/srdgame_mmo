#ifndef NPC_INFO_H_
#define NPC_INFO_H_

namespace srdgame
{

struct NpcProp
{
	int _str;
	int _agi;
	int _vit;
	int _int;
	int _dex;
	int _luk;
	int _max_hp;
	int _max_sp;
	int _cur_hp;
	int _cur_sp;
	int _karma;
	int _manner;
};
struct NpcShow
{
	int _class; // Face class.
	uint16 _slot;
	uint8 _sex;
	int _hair_style;
	int _hair_color;
	int _clothes_color;
	int _head_top;
	int _head_middle;
	int _head_bottom;
	int _shield;
	bool _show_equid;
};

// Npc could have exp, and increase their level.
struct NpcExp
{
	int _base_lvl;
	int _job_lvl;
	int _base_exp;
	int _job_exp;
	int _zeny;
};

class NpcInfo
{
public:
	NpcInfo(int id, int cl, std::string name) :
	_id(id), _class(cl), _name(name)
	{
	}
	virtual ~NpcInfo(){}

	inline int get_id()
	{
		return _id;
	}
	inline int get_class()
	{
		return _class;
	}
	inline const string& get_name()
	{
		return _name;
	}

protected:
	int _id;
	int _class;
	std::string _name;
}

}

#endif
