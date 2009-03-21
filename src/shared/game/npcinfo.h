#ifndef NPC_INFO_H_
#define NPC_INFO_H_

namespace srdgame
{

struct NpcProp
{
	uint32 _str;
	uint32 _agi;
	uint32 _vit;
	uint32 _int;
	uint32 _dex;
	uint32 _luk;
	uint32 _max_hp;
	uint32 _max_sp;
	uint32 _cur_hp;
	uint32 _cur_sp;
	uint32 _karma;
	uint32 _manner;
};
struct NpcShow
{
	uint32 _class; // Face class.
	uint16 _slot;
	uint8 _sex;
	uint32 _hair_style;
	uint32 _hair_color;
	uint32 _clothes_color;
	uint32 _head_top;
	uint32 _head_middle;
	uint32 _head_bottom;
	bool _show_equid;
};
struct NpcExp
{
	uint32 _base_lvl;
	uint32 _job_lvl;
	uint32 _base_exp;
	uint32 _job_exp;
	uint32 _zeny;
	uint32 _prop_point;
	uint32 _skill_point;
};

struct NpcInfo
{
	uint32 _id;
	uint32 _acc_id;
}
}

#endif
