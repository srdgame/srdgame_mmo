#include "charmgr.h"
#include "ro_defs.h"
#include "rosql.h"
#include "log.h"

using namespace std;
using namespace ro;
using namespace srdgame;

#ifndef CHAR_MGR_DEBUG
#define CHAR_MGR_DEBUG
#undef _LogDebug_
#endif

#ifdef CHAR_MGR_DEBUG
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

#define LN "CHAR_MGR"

#define CHAR_CONF_DEFAULT "./data/conf/char_default.txt"

CharMgr::CharMgr() : _config(CHAR_CONF_DEFAULT)
{
	_sql = new RoSql();
}

CharMgr::~CharMgr()
{
	delete _sql;
}

RoCharInfo* CharMgr::create_new(CreateCharData* data, int account_id)
{
	if (account_id == 0)
		return NULL;

	RoCharInfo* info = new RoCharInfo();
	//memset((char*)info, 0, sizeof(RoCharInfoBase));
	info->_name = string(data->_name);
	info->_id = _sql->get_max_char_id();
	info->_account_id = account_id;
	info->_prop._str = data->_str;
	info->_prop._agi = data->_agi;
	info->_prop._vit = data->_vit;
	info->_prop._int = data->_int;
	info->_prop._dex = data->_dex;
	info->_prop._luk = data->_luk;
	info->_slot = data->_slot; // The slot index for select characters.... -_-!
	info->_show._hair_color = data->_hair_color;
	info->_show._hair_style = data->_hair_style;

	// TODO: more default stuff
	info->_show._class = 0; // JOB_NOVICE
	info->_class = 0; // JOB_NOVICE
	info->_exp._base_lvl = 1;
	info->_exp._job_lvl = 1;
	info->_exp._base_exp = 0;
	info->_exp._job_exp = 0;
	info->_exp._zeny = _config.get_value<int>("zeny");	
	info->_exp._prop_point = 0;
	info->_exp._skill_point = 0;

	// for hp and sp.
	info->_prop._max_hp = _config.get_value<int>("hp_vit_rate");
	if (info->_prop._max_hp == 0)
		info->_prop._max_hp = 1;
	int i =  _config.get_value<int>("hp_max");
	if (i == 0)
		i = 40;
	info->_prop._max_hp = i * (100 + info->_prop._vit * info->_prop._max_hp) / 100;

	info->_prop._max_sp = _config.get_value<int>("sp_int_rate");
	if (info->_prop._max_sp == 0)
		info->_prop._max_sp = 1;
	i =  _config.get_value<int>("sp_max");
	if (i == 0)
		i = 11;
	info->_prop._max_sp = i * (100 + info->_prop._int * info->_prop._max_sp) / 100;

	info->_prop._cur_hp = info->_prop._max_hp;
	info->_prop._cur_sp = info->_prop._max_sp;

	info->_items.resize(2);
	info->_items[0]._type = _config.get_value<int>("start_weapon");
	info->_items[0]._amount = 1;
	info->_items[0]._identify = 1;
	info->_items[1]._type = _config.get_value<int>("start_armor");
	LogDebug("CHAR_MGR", "Start_weapon : %d, \t start_aramor : %d", info->_items[0]._id, info->_items[1]._id);	
	info->_items[1]._amount = 1;
	info->_items[1]._identify = 1;

	string map = _config.get_value<string>("default_map");
	memcpy(info->_last_pos._map_name, map.c_str(), MAX_MAP_NAME_LEN);
	info->_last_pos._x = _config.get_value<int>("default_pos_x");
	info->_last_pos._y = _config.get_value<int>("default_pos_y");

	//("RealmServer", "Require to create char :%d", info->_id);
	_sql->save_char(_sql->get_max_char_id(), *info);
	return info;
}
size_t CharMgr::load_chars(int account_id, RoCharInfoBase*& chars)
{
	return _sql->load_chars(account_id, chars);
	//_LogDebug_(LN, "%s characters have been loaded", count);
	//return count;
}

bool CharMgr::load_char_detail(int char_id, RoCharInfo& info)
{
	return _sql->load_char(char_id, info);
}
