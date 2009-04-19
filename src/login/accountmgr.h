#ifndef ACCOUNT_MGR_H_
#define ACCOUNT_MGR_H_

#include "singleton.h"
#include <string>
#include <map>
#include "mutex.h"

namespace ro
{
class RoSql;
}
namespace srdgame
{
class LoginSocket;
struct Packet;
struct UserInfo;
class AccountMgr : public Singleton < AccountMgr >
{
public:
	AccountMgr();
	~AccountMgr();

public:
	bool create_new(UserInfo* info);

	UserInfo* load_user(std::string name);
	UserInfo* load_user(long id);

	void release_user(long id);

protected:
	ro::RoSql* _sql;

	std::map<long, UserInfo> _users;
	Mutex _users_lock;

	long _max_id;
	Mutex _max_id_lock;
};
}

#endif
