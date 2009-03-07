// This will mention all the data of realm 

#ifndef REALM_MANAGER_H_
#define REALM_MANAGER_H_

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

	void enum_login_servers(std::vector<LoginSrvInfo>& info);

	void add_login_server(LoginSocket* s);
	void remove_login_server(LoginSocket* s);

	void update_login_server(LoginSocket* s, LoginSrvInfo& info);
	void update_login_server_name(LoginSocket* s, std::string name);
	void update_login_server_info(LoginSocket* s, std::string info);
	void update_login_server_status(LoginSocket* s, LoginSrvStatus status);

	void add_client(LoginSocket* s);
	void remove_client(LoginSocket* s);

protected:
	std::map<SOCKET, LoginSocket*> _servers;
	std::map<LoginSocket*, LoginSrvInfo> _servers_info;
	std::map<SOCKET, LoginSocket*> _clients;
	Mutex _server_lock;
	Mutex _client_lock;
};

}


#endif
