#include "rosql.h"
#include "rouser.h"
#include <string>
#include <cstring>
#include "log.h"
#include "databasemgr.h"
#include "conversion_cast.h"
#include "queryresult.h"

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

bool RoSql::load_account(uint32 id, UserInfoEx<RoUser>& info)
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
uint32 RoSql::get_max_id()
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

	uint32 max = f[0].get<uint32>();
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
	user->_state = fields[6].get<uint32>();
	user->_unban_time = fields[7].get<uint32>();
	user->_expire_time = fields[8].get<uint32>();
	user->_login_count = fields[9].get<uint16>();
	memcpy(user->_last_login, fields[10].get<char*>(), sizeof(user->_last_login));
	memcpy(user->_last_ip, fields[11].get<char*>(), sizeof(user->_last_ip));
	_LogDebug_(LN, "Fetching completed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	return true;
}
