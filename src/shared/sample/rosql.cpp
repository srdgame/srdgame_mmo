#include "rosql.h"
#include "rouser.h"
#include <string>
#include <cstring>
#include "log.h"
#include "databasemgr.h"
#include "conversion_cast.h"
#include "queryresult.h"
#include "metadefs.h"
#include "roitemdb.h"

using namespace std;
using namespace ro;
using namespace srdgame;


#define _LOGIN_AUTH_DEBUG_
#undef _LogDebug_

#ifdef _LOGIN_AUTH_DEBUG_
#define _LogDebug_ LogDebug
#else
#define _LogDebug_ //
#endif

#undef LN
#define LN "RoSql"


static const string RO_DB_CONF = "ro_db.conf";

#define RO_ACCOUNT_TB "login"
#define RO_ACCOUNT_REG_TB "global_reg_value"
#define RO_CHAR_TB "char"
#define RO_ITEM_TB "inventory"
#define RO_CART_TB "cart_inventory"
#define RO_SKILL_TB "skill"
#define RO_FRIEND_TB "friends"
#define RO_HOTKEY_TB "hotkey"

#define MAKE_STRING(x) ((conversion_cast<std::string>(x)).c_str())

RoSql::RoSql()
{
}

RoSql::~RoSql()
{
}

bool RoSql::create_account(UserInfoEx<RoUser>& info)
{
	RoUser* user = &info._ex;
	if (info._id == 0)
	{
		LogError("rosql", "could not create account with id == 0");
		return false;
	}
	
	// TODO: User script to generate the sql string by parse the user data to script..
	std::string sql = "INSERT INTO `%s` (`account_id`, `userid`, `user_pass`, `sex`, `email`, `level`, `state`, `unban_time`, `expiration_time`, `logincount`, `lastlogin`, `last_ip`) VALUES (%s, '%s', '%s', '%s', '%s', %s, %s, %s, %s, %s, '%s', '%s')";

	DatabaseMgr::get_singleton().execute(sql.c_str(), 
			RO_ACCOUNT_TB,
			MAKE_STRING(info._id),//(conversion_cast<string>(info._id)).c_str(),
			info._name.c_str(), 
			info._pass.c_str(),
			(user->_sex == 'M' ? "M" : "F"),
			(/*user->_email != NULL ? user->_email :*/ "x@x.com"),
		"0",//	MAKE_STRING(user->_level),//(conversion_cast<string>(user->_level)).c_str(),
			MAKE_STRING(user->_state),//(conversion_cast<string>(user->_state)).c_str(),
			MAKE_STRING(user->_unban_time),//(conversion_cast<string>(user->_unban_time)).c_str(),
			MAKE_STRING(user->_expire_time),//(conversion_cast<string>(user->_expire_time)).c_str(),
			MAKE_STRING(user->_login_count),//(conversion_cast<string>(user->_login_count)).c_str(),
			(/*user->_last_login != NULL ? user->_last_login :*/ "0000-00-00 00:00:00"),
			(/*user->_last_ip != NULL ? user->_last_ip :*/ ""));

	// Wait for a while.
	//
	usleep(100000);

	// TODO: Make this working later.
/*
	// Delete from arg table.
	sql = "DELETE FROM `%s` WHERE `type`='1' AND `account_id`='%d'";

	DatabaseMgr::get_singleton().execute(sql.c_str(),
			RO_ACCOUNT_REG_TB,
			MAKE_STRING(info._id));
	
	sql = "INSERT INTO `%s` (`type`, `account_id`, `str`, `value`) VALUES ( 1 , '%d' , %s , %s );";
	DatabaseMgr::get_singleton().execute(sql.c_str(), RO_ACCOUNT_REG_TB, "", "");

*/
	return false;
}

bool RoSql::load_account(int id, UserInfoEx<RoUser>& info)
{
	// TODO: Specify the columns. "SELECT `account_id`,`userid`,`user_pass`,`sex`,`email`,`level`,`state`,`unban_time`,`expiration_time`,`logincount`,`lastlogin`,`last_ip` FROM `%s` WHERE `account_id` = %d",
	//
	std::string sql = "SELECT * FROM `%s` WHERE `account_id`= '%s'";

	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(),
		RO_ACCOUNT_TB, MAKE_STRING(id));
	if (res->get_row_size() != 1)
	{
		res->Delete();
		_LogDebug_(LN, "No account with id : %d ", id);
		return false;
	}
	Field* field = res->fetch();
	if (field)
	{
		if (res->get_field_size() != 12)
		{
			LogError(LN, "Incorrect result has been queried out!!!!!!!!!!");
			res->Delete();
			return false;
		}
		fetch_account_info(field, info);
	}
	res->Delete();
	return true;
}
int RoSql::get_max_id()
{
	std::string sql = "SELECT MAX(`account_id`)+1 FROM `%s`";

	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(),
			RO_ACCOUNT_TB);
	if (res == NULL)
	{
		return 1000;
	}
	
	Field* f = res->fetch();
	if (f == NULL)
	{
		res->Delete();
		return 1000;
	}

	int max = f[0].get<int>();
	res->Delete();
	return max;
}
bool RoSql::load_account(const char* name, UserInfoEx<RoUser>& info)
{
	_LogDebug_(LN, "Starting to select from DB...................");
	std::string sql = "SELECT `account_id` FROM `%s` WHERE `userid`= %s '%s'";

	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(),
		RO_ACCOUNT_TB, "BINARY", name);
	if (!res || res->get_row_size() != 1)
	{
		if (res)
			res->Delete();
		_LogDebug_(LN, "No account with name : %s ", name);
		return false;
	}
	Field* field = res->fetch();
	if (field)
	{
		long id = field->get<long>();
		bool result =  load_account(id, info);
		res->Delete();
		return result;
	}
	res->Delete();
	return false;
}

bool RoSql::fetch_account_info(Field* fields, UserInfoEx< RoUser >& info)
{
	_LogDebug_(LN, "Fetching Datas.............................");
	info._id = fields[0].get<long>();
	info._name = fields[1].get<std::string>();
	info._pass = fields[2].get<std::string>();
	RoUser* user = &info._ex;
	
	user->_sex = fields[3].get<char>();
	memcpy(user->_email, fields[4].get<char*>(), sizeof(user->_email));
	user->_level = fields[5].get<uint8>();
	user->_state = fields[6].get<int>();
	user->_unban_time = fields[7].get<int>();
	user->_expire_time = fields[8].get<int>();
	user->_login_count = fields[9].get<uint16>();
	memcpy(user->_last_login, fields[10].get<char*>(), sizeof(user->_last_login));
	memcpy(user->_last_ip, fields[11].get<char*>(), sizeof(user->_last_ip));
	_LogDebug_(LN, "Fetching completed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	return true;
}
int RoSql::get_max_char_id()
{
	std::string sql = "SELECT MAX(`char_id`)+1 FROM `%s`";

	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(),
			RO_CHAR_TB);
	if (res == NULL)
	{
		_LogDebug_(LN, "NO result when select max char_id");
		return 1000;
	}
	
	Field* f = res->fetch();
	if (f == NULL)
	{
		res->Delete();
		return 1000;
	}

	int max = f[0].get<int>();
	if (max < 1000)
		max = 1000;
	_LogDebug_(LN, "Max char_id is %d", max);
	res->Delete();
	return max;

}
size_t RoSql::load_chars(int account_id, RoCharInfoBase*& result)
{
	std::string sql = "SELECT `char_id`,`char_num`,`name`,`class`,`base_level`,`job_level`,`base_exp`,`job_exp`,`zeny`,`str`,`agi`,`vit`,`int`,`dex`,`luk`,`max_hp`,`hp`,`max_sp`,`sp`,`status_point`,`skill_point`,`option`,`karma`,`manner`,`hair`,`hair_color`,`clothes_color`,`weapon`,`shield`,`head_top`,`head_mid`,`head_bottom` FROM `%s` WHERE `account_id`='%d'";
	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(),
			RO_CHAR_TB, account_id);
	if (!res)
		return 0;

	size_t count = res->get_row_size();
	if (count == 0)
	{
		res->Delete();
		return 0;
	}
	result = new RoCharInfoBase[count];

	size_t i = 0;
	for (; i < count; ++i)
	{
		Field* f = res->fetch();
		if (!f)
			break;
		RoCharInfoBase* info =result + i;
		info->_account_id = account_id;
		if (!fetch_chars_info(f, *info))
			break;
		res->next();
	}
	res->Delete();
	return i;
}

bool RoSql::load_char(int char_id, RoCharInfo& info, bool load_everything)
{
	info._id = char_id;
	std::string sql = "SELECT `account_id`,`char_num`,`name`,`class`,`base_level`,`job_level`,`base_exp`,`job_exp`,`zeny`,`str`,`agi`,`vit`,`int`,`dex`,`luk`,`max_hp`,`hp`,`max_sp`,`sp`,`status_point`,`skill_point`,`option`,`karma`,`manner`,`party_id`,`guild_id`,`pet_id`,`homun_id`,`hair`,`hair_color`,`clothes_color`,`weapon`,`shield`,`head_top`,`head_mid`,`head_bottom`,`last_map`,`last_x`,`last_y`,`save_map`,`save_x`,`save_y`,`partner_id`,`father`,`mother`,`child`,`fame` FROM `%s` WHERE `char_id`=%d LIMIT 1";
	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(),
			RO_CHAR_TB, char_id);
	if (!res)
	{
		//_LogDebug_("RO_SQL", "Failed to query!!!!!!!!!!!!!");
		return false;
	}

	size_t count = res->get_row_size();
	if (count != 1)
	{
		//_LogDebug_("RO_SQL", "count is not one, means no invald data");
		res->Delete();
		return false;
	}
	Field * f = res->fetch();
	if (!f)
	{
		//_LogDebug_("RO_SQL", "No data fields~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		res->Delete();
		return false;
	}
	fetch_char_info(f, info);
	res->Delete();
	if (!load_everything)
	{
		return true;
	}

	// Or we will load other stuff
	// Load memory data.
	
	// load items.
	if (!load_items(char_id, info._items))
		return false;

	// Load cart
	if (!load_cart(char_id, info._carts))
		return false;

	// load storage
	
	// load skill
	if (!load_skill(char_id, info._skills))
		return false;

	// load friends
	if (!load_friends(char_id, info._friends))
		return false;

	// load hotkey
	if (!load_hotkey(char_id, info._hotkeys))
		return false;

	// We are ok here.
	return true;
}
bool RoSql::save_char(int char_id, RoCharInfo& info)
{
	if (char_id != info._id)
	{
		LogError("RO", "Incorrect char id provided, the id are not equal. char_id : %d, info._id : %d", char_id, info._id);
		return false;
	}
	LogNotice("RO", "Saving char info~~~~~~~~~~~~~~~~~~~~~~");
	// Insert one new row
	std::string row_sql = "INSERT INTO `%s` (`char_id`, `account_id`, `char_num`, `name`, `zeny`, `str`, `agi`, `vit`, `int`, `dex`, `luk`, `max_hp`, `hp`,"
		"`max_sp`, `sp`, `hair`, `hair_color`, `last_map`, `last_x`, `last_y`, `save_map`, `save_x`, `save_y`) VALUES ("
		"'%d', '%d', '%d', '%s', '%d',  '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d','%d', '%d','%d', '%d', '%s', '%d', '%d', '%s', '%d', '%d')";
	DatabaseMgr::get_singleton().execute(row_sql.c_str(),
			RO_CHAR_TB, info._id, info._account_id, info._slot, info._name.c_str(), info._exp._zeny, info._prop._str, info._prop._agi,
			info._prop._vit, info._prop._int, info._prop._dex, info._prop._luk, info._prop._max_hp, info._prop._cur_hp,
			info._prop._max_sp, info._prop._cur_sp, info._show._hair_style, info._show._hair_color, info._last_pos._map_name,
			info._last_pos._x, info._last_pos._y, info._save_pos._map_name, info._save_pos._x, info._save_pos._y);

	// save items.
	save_items(char_id, info._items);
	// save card data
	// save storage data
	// save char info
	std::string char_sql = "UPDATE `%s` SET `base_level`='%d', `job_level`='%d',"
		"`base_exp`='%u', `job_exp`='%u', `zeny`='%d',"
		"`max_hp`='%d',`hp`='%d',`max_sp`='%d',`sp`='%d',`status_point`='%d',`skill_point`='%d',"
		"`str`='%d',`agi`='%d',`vit`='%d',`int`='%d',`dex`='%d',`luk`='%d',"
		"`option`='%d',`party_id`='%d',`guild_id`='%d',`pet_id`='%d',`homun_id`='%d',"
		"`weapon`='%d',`shield`='%d',`head_top`='%d',`head_mid`='%d',`head_bottom`='%d',"
		"`last_map`='%s',`last_x`='%d',`last_y`='%d',`save_map`='%s',`save_x`='%d',`save_y`='%d'"
		" WHERE  `account_id`='%d' AND `char_id` = '%d'";

	DatabaseMgr::get_singleton().execute(char_sql.c_str(),
			RO_CHAR_TB, info._exp._base_lvl, info._exp._job_lvl,
			info._exp._base_exp, info._exp._job_exp, info._exp._zeny,
			info._prop._max_hp, info._prop._cur_hp, info._prop._max_sp, info._prop._cur_sp, info._exp._prop_point, info._exp._skill_point,
			info._prop._str, info._prop._agi, info._prop._vit, info._prop._int, info._prop._dex, info._prop._luk,
			info._option, info._party_id, info._guild_id, info._pet_id, info._homun_id,
			info._show._weapon, info._show._shield, info._show._head_top, info._show._head_middle, info._show._head_bottom,
			info._last_pos._map_name, info._last_pos._x, info._last_pos._y, info._save_pos._map_name, info._save_pos._x, info._save_pos._y,
			info._account_id, info._id);

	// other seldom stuff.
	string other_sql = "UPDATE `%s` SET `class`='%d',"
		"`hair`='%d',`hair_color`='%d',`clothes_color`='%d',"
		"`partner_id`='%d', `father`='%d', `mother`='%d', `child`='%d',"
		"`karma`='%d',`manner`='%d', `fame`='%d'"
		" WHERE  `account_id`='%d' AND `char_id` = '%d'";
	DatabaseMgr::get_singleton().execute(other_sql.c_str(),
		RO_CHAR_TB, info._class,
		info._show._hair_style, info._show._hair_color, info._show._clothes_color,
		info._family._partner_id, info._family._father_id, info._family._mother_id, info._family._child_id,
		info._prop._karma, info._prop._manner, info._fame,
		info._account_id, info._id);

	LogSuccess("RO", "Save char completed!!!!");
	// Major save?
	// Memo points.
	//
	// Skill
	// Friends
	// hotkeys
	//
	//
	return true;
}

bool RoSql::load_items(int char_id, std::vector<RoCharItem>& items)
{
	items.clear();

	string sql = "SELECT `id`, `nameid`, `amount`, `equip`, `identify`, `refine`, `attribute`, `expire_time`";
	for (int i = 0; i <MaxSlotCount; ++i)
		sql += ", `card" + conversion_cast<string>(i) + "`";
	sql += " FROM `%s` WHERE `char_id`=%d LIMIT %d";
	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(), RO_ITEM_TB, char_id, MaxItemCount);

	//LogError("RoSql", "AAAAAAAAAAAAAAAAAAAAAAAAA");
	if (res == NULL)
		return true;

	//LogError("RoSql", "aaaaaaaaaaaaaaaaaaaaaa");
	size_t count = res->get_row_size();
	for (size_t n = 0; n < count; ++n)
	{
		Field* f = res->fetch();
		RoCharItem info;
		info._id = f[0].get<int>();
		info._type = f[1].get<short>();
		info._amount = f[2].get<short>();
		info._equip = f[3].get<short>();
		info._identify = f[4].get<int>();
		info._refine = f[5].get<int>();
		info._attrs = f[6].get<int>();
		info._expire_time = f[7].get<unsigned int>();
		// for slots.
		for (int i = 0; i < MaxSlotCount; ++i)
		{
			info._cards[i] = f[7 + i].get<short>();
		}
		items.push_back(info);
		res->next();
	}
	res->Delete();

	return true;
}

bool RoSql::save_items(int char_id, const std::vector<RoCharItem>& items)
{
	string sql = "INSERT INTO `%s`(`%s`, `nameid`, `amount`, `equip`, `identify`, `refine`, `attribute`, `expire_time`";
	for (int i = 0; i <MaxSlotCount; ++i)
		sql += ", `card" + conversion_cast<string>(i) + "`";
	sql += ") VALUES ('%d', '%d', '%d', '%d', '%d', '%d', '%d', '%u'";
	for (int i = 0; i < MaxSlotCount; ++i)
		sql += ", '%d'";
	sql += ")";
	size_t count = items.size();
	for (size_t n = 0; n < count; ++n)
	{
		DatabaseMgr::get_singleton().execute(sql.c_str(),
			RO_ITEM_TB, "char_id", char_id, items[n]._type, items[n]._amount, items[n]._equip, items[n]._identify, items[n]._refine, items[n]._attrs, items[n]._expire_time, items[n]._cards[0], items[n]._cards[1], items[n]._cards[2], items[n]._cards[3]);
	}
	return true;
}

bool RoSql::load_mem_data(int char_id, ...)
{
	return true;
}
bool RoSql::save_mem_data(int char_id, ...)
{
	return true;
}

bool RoSql::load_cart(int char_id, std::vector<RoCharItem>& cart_items)
{
	cart_items.clear();

	string sql = "SELECT `id`, `nameid`, `amount`, `equip`, `identify`, `refine`, `attribute`, `expire_time`";
	for (int i = 0; i <MaxSlotCount; ++i)
		sql += ", `card" + conversion_cast<string>(i) + "`";
	sql += " FROM `%s` WHERE `char_id`=%d LIMIT %d";
	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(), RO_CART_TB, char_id, MaxCartCount);

	if (res == NULL)
		return true;

	size_t count = res->get_row_size();
	for (size_t n = 0; n < count; ++n)
	{
		Field* f = res->fetch();
		RoCharItem info;
		info._id = f[0].get<int>();
		info._type = f[1].get<short>();
		info._amount = f[2].get<short>();
		info._equip = f[3].get<uint16>();
		info._identify = f[4].get<char>();
		info._refine = f[5].get<char>();
		info._attrs = f[6].get<char>();
		info._expire_time = f[7].get<unsigned int>();
		cart_items.push_back(info);
		res->next();
	}
	res->Delete();

	return true;
}
bool RoSql::save_cart(int char_id, const std::vector<RoCharItem>& cart_items)
{
	return true;
}

bool RoSql::load_storage(int char_id, ...)
{
	return true;
}
bool RoSql::save_storage(int char_id, ...)
{
	return true;
}

bool RoSql::load_skill(int char_id, std::vector<RoCharSkillInfo>& skills)
{
	skills.clear();

	string sql = "SELECT `id`, `lv` FROM `%s` WHERE `char_id`=%d LIMIT %d";
	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(), RO_SKILL_TB, char_id, MaxSkillCount);

	if (res == NULL)
		return true;

	size_t count = res->get_row_size();
	for (size_t n = 0; n < count; ++n)
	{
		Field* f = res->fetch();
		RoCharSkillInfo skill;
		skill._id = f[0].get<int>();
		skill._lvl = f[1].get<int>();
		skill._flag = f[2].get<int>();

		skills.push_back(skill);
		res->next();
	}
	res->Delete();

	return true;
}
bool RoSql::save_skill(int char_id, const std::vector<RoCharSkillInfo>& skills)
{
	return true;
}

bool RoSql::load_friends(int char_id, std::vector<RoCharFriendInfo>& friends)
{
	friends.clear();

	string sql = "SELECT c.`account_id`, c.`char_id`, c.`name` FROM `%s` c LEFT JOIN `%s` f ON f.`friend_account` = c.`account_id` AND f.`friend_id` = c.`char_id` WHERE f.`char_id`=%d LIMIT %d";
	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(), RO_CHAR_TB, RO_FRIEND_TB, char_id, MaxFriendCount);

	if (res == NULL)
		return true;

	size_t count = res->get_row_size();
	for (size_t n = 0; n < count; ++n)
	{
		Field* f = res->fetch();
		RoCharFriendInfo fr;
		fr._account_id = f[0].get<int>();
		fr._char_id = f[1].get<int>();
		fr._name = f[2].get<string>();
		fr._nick_name = f[2].get<string>();

		friends.push_back(fr);
		res->next();
	}
	res->Delete();
	return true;
}
bool RoSql::save_friends(int char_id, const std::vector<RoCharFriendInfo>& friends)
{
	return true;
}

bool RoSql::load_hotkey(int char_id, std::vector<RoCharHotKey>& keys)
{
	keys.clear();

	string sql = "SELECT `hotkey`, `type`, `itemskill_id`, `skill_lvl` FROM `%s` WHERE `char_id`=%d LIMIT %d";
	QueryResult* res = DatabaseMgr::get_singleton().query(sql.c_str(), RO_HOTKEY_TB, char_id, MaxHotKeyCount);

	if (res == NULL)
		return true;;

	size_t count = res->get_row_size();
	for (size_t n = 0; n < count; ++n)
	{
		Field* f = res->fetch();
		RoCharHotKey key;
		key._id = f[0].get<int>();
		key._type = f[1].get<uint16>();
		key._lvl = f[2].get<uint16>();

		keys.push_back(key);
		res->next();
	}
	res->Delete();

	return true;
}
bool RoSql::save_hotkey(int char_id, const std::vector<RoCharHotKey>& keys)
{
	return true;
}
bool RoSql::fetch_chars_info(Field* f, RoCharInfoBase& info)
{
	/*if (32 != f->get_field_size())
		return false;*/

	info._id = f[0].get<int>();
	info._slot = f[1].get<int>();
	info._name = f[2].get<std::string>();
	info._class = f[3].get<int>();
	info._exp._base_lvl = f[4].get<int>();
	info._exp._job_lvl = f[5].get<int>();
	info._exp._base_exp = f[6].get<int>();
	info._exp._job_exp = f[7].get<int>();
	info._exp._zeny = f[8].get<int>();
	info._prop._str = f[9].get<int>();
	info._prop._agi = f[10].get<int>();
	info._prop._vit = f[11].get<int>();
	info._prop._int = f[12].get<int>();
	info._prop._dex = f[13].get<int>();
	info._prop._luk = f[14].get<int>();
	info._prop._max_hp = f[15].get<int>();
	info._prop._cur_hp = f[16].get<int>();
	info._prop._max_sp = f[17].get<int>();
	info._prop._cur_sp = f[18].get<int>();
	info._exp._prop_point = f[19].get<int>();
	info._exp._skill_point = f[20].get<int>();
	info._option = f[21].get<int>();
	info._prop._karma = f[22].get<int>();
	info._prop._manner = f[23].get<int>();
	info._show._hair_style = f[24].get<int>();
	info._show._hair_color = f[25].get<int>();
	info._show._clothes_color = f[26].get<int>();
	info._show._weapon = f[27].get<int>();
	info._show._shield = f[28].get<int>();
	info._show._head_top = f[29].get<int>();
	info._show._head_middle = f[30].get<int>();
	info._show._head_bottom = f[31].get<int>();
	return true;
}

bool RoSql::fetch_char_info(Field* f, RoCharInfo& info)
{
	/*if (46 != f->get_field_size())
		return false;*/

	info._account_id = f[0].get<int>();
	//LogDebug("AAAAAAAAAAAA", f[2].get<string>().c_str());
	info._slot = f[1].get<int>();
	info._name = f[2].get<string>();
	//LogDebug("AAAAAAAAAAAA", info._name.c_str()); // TODO: the name geting is crashing....
	info._class = f[3].get<int>();
	info._exp._base_lvl = f[4].get<int>();
	info._exp._job_lvl = f[5].get<int>();
	info._exp._base_exp = f[6].get<int>();
	info._exp._job_exp = f[7].get<int>();
	info._exp._zeny = f[8].get<int>();
	info._prop._str = f[9].get<int>();
	info._prop._agi = f[10].get<int>();
	info._prop._vit = f[11].get<int>();
	info._prop._int = f[12].get<int>();
	info._prop._dex = f[13].get<int>();
	info._prop._luk = f[14].get<int>();
	info._prop._max_hp = f[15].get<int>();
	info._prop._cur_hp = f[16].get<int>();
	info._prop._max_sp = f[17].get<int>();
	info._prop._cur_sp = f[18].get<int>();
	info._exp._prop_point = f[19].get<int>();
	info._exp._skill_point = f[20].get<int>();
	info._option = f[21].get<int>();
	info._prop._karma = f[22].get<int>();
	info._prop._manner = f[23].get<int>();
	info._party_id = f[24].get<int>();
	info._guild_id = f[25].get<int>();
	info._pet_id = f[26].get<int>();
	info._homun_id = f[27].get<int>();
	info._show._hair_style = f[28].get<int>();
	info._show._hair_color = f[29].get<int>();
	info._show._clothes_color = f[30].get<int>();
	info._show._weapon = f[31].get<int>();
	info._show._shield = f[32].get<int>();
	info._show._head_top = f[33].get<int>();
	info._show._head_middle = f[34].get<int>();
	info._show._head_bottom = f[35].get<int>();
	memcpy(info._last_pos._map_name, f[36].get<char*>(), sizeof(info._last_pos._map_name));
	//LogDebug("ROSQL", "Last map name : %s", info._last_pos._map_name);
	info._last_pos._x = f[37].get<int>();
	info._last_pos._y = f[38].get<int>();
	info._last_pos._z = 0;
	info._last_pos._r = 0;
	memcpy(info._save_pos._map_name, f[39].get<char*>(), sizeof(info._save_pos._map_name));
	info._save_pos._x = f[40].get<int>();
	info._save_pos._y = f[41].get<int>();
	info._save_pos._z = info._save_pos._r = 0;
	info._family._partner_id = f[42].get<int>();
	info._family._father_id = f[43].get<int>();
	info._family._mother_id = f[44].get<int>();
	info._family._child_id = f[45].get<int>();
	info._fame = f[46].get<int>();

	return true;
}
