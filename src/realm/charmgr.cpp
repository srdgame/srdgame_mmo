#include "charmgr.h"
#include "ro_defs.h"
#include "rosql.h"

using namespace std;
using namespace ro;
using namespace srdgame;


CharMgr::CharMgr()
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

	RoCharInfo* info = new RoCharInfo;
	//memset((char*)info, 0, sizeof(RoCharInfo));
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

	//("RealmServer", "Require to create char :%d", info->_id);
	_sql->save_char(_sql->get_max_char_id(), *info);
	return info;
}
size_t CharMgr::load_chars(int account_id, RoCharInfo*& chars)
{
	return _sql->load_chars(account_id, chars);
}


