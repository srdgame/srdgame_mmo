#ifndef NPC_INFO_H_
#define NPC_INFO_H_
#include <string>
namespace srdgame
{

struct NpcProp
{
	NpcProp() : _str(0), _agi(0), _vit(0), _int(0), _dex(0), _luk(0), _max_hp(1), _cur_hp(1), _max_sp(0), _cur_sp(0), _karma(0), _manner(0)
	{
	}
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
	NpcShow() : _class(0), _sex(0), _hair_style(0), _hair_color(0), _clothes_color(0), _head_top(0), _head_middle(0), _head_bottom(0), _weapon(0), _shield(0), _show_equid(true)
	{
	}
	int _class; // Face class.
	char _sex;
	int _hair_style;
	int _hair_color;
	int _clothes_color;
	int _head_top;
	int _head_middle;
	int _head_bottom;
	int _weapon;
	int _shield;
	bool _show_equid;
};

// Npc could have exp, and increase their level.
struct NpcExp
{
	NpcExp() : _base_lvl(1), _job_lvl(0), _base_exp(0), _job_exp(0), _zeny(0)
	{
	}
	int _base_lvl;
	int _job_lvl;
	int _base_exp;
	int _job_exp;
	int _zeny;
};

struct NpcInfo
{
	NpcInfo() : _id(0), _class(0)
	{
	}
	int _id;
	int _class;
	std::string _name;
};

}

#endif
