#include "accountmgr.h"
#include "ro_defs.h"
#include "rosql.h"
#include "log.h"

using namespace srdgame;
using namespace ro;
using namespace std;

AccountMgr::AccountMgr() : _max_id(0)
{
	_sql = new RoSql();
}

AccountMgr::~AccountMgr()
{
	_users.clear();
	delete _sql;
}

bool AccountMgr::create_new(UserInfo* info)
{
	UserInfoEx<RoUser>* ro_info = static_cast<UserInfoEx< RoUser>* >(info);
	if (ro_info == NULL)
		return false;;

	if (info->_id == 0)
	{
		_max_id = _sql->get_max_id();
		info->_id = _max_id;
	}
	return _sql->create_account(*ro_info);
}

UserInfo* AccountMgr::load_user(std::string name)
{
	UserInfoEx< RoUser> ro_info;
	UserInfo* info = NULL;
	if (_sql->load_account(name.c_str(), ro_info))
	{
		//LogDebug("Account", "User Has Been Loaded, We are wininig!!!!");
		_users_lock.lock();
		_users[ro_info._id] = ro_info;
		info = &_users[ro_info._id];
		_users_lock.unlock();
	}
	LogSuccess("Account", "We are here waiting for you!!!");	
	return info;
}

UserInfo* AccountMgr::load_user(long id)
{
	UserInfoEx< RoUser> ro_info;
	UserInfo* info = NULL;
	if (_sql->load_account((uint32)id, ro_info))
	{
		_users_lock.lock();
		_users[info->_id] = ro_info;
		info = &_users[info->_id];
		_users_lock.lock();
	}
	
	return info;
}
void AccountMgr::release_user(long id)
{
	_users.erase(id);
}

