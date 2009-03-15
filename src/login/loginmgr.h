// This will mention all the data of realm 

#ifndef LOGIN_MANAGER_H_
#define LOGIN_MANAGER_H_

#include "singleton.h"
#include <vector>
#include <map>

#include "socketdefs.h"
#include "mutex.h"
#include "typedefs.h"

namespace srdgame
{
class LoginSocket;
class LoginMgr : public Singleton < LoginMgr >
{
public:
	LoginMgr();
	virtual ~LoginMgr();

	void enum_realm_servers(std::vector<RealmSrvInfo>& info);

	void add_realm_server(LoginSocket* s);
	void remove_realm_server(LoginSocket* s);

	void update_realm_server(LoginSocket* s, RealmSrvInfo& info);
	void update_realm_server_name(LoginSocket* s, std::string name);
	void update_realm_server_info(LoginSocket* s, std::string info);
	void update_realm_server_status(LoginSocket* s, RealmSrvStatus status);

	void add_client(LoginSocket* s);
	void remove_client(LoginSocket* s);

protected:
	std::map<SOCKET, LoginSocket*> _servers;
	std::map<LoginSocket*, RealmSrvInfo> _servers_info;
	std::map<SOCKET, LoginSocket*> _clients;
	Mutex _server_lock;
	Mutex _client_lock;
};

}


#endif
