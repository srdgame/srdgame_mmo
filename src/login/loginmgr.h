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
class LoginSocketBase;
class LoginMgr : public Singleton < LoginMgr >
{
public:
	LoginMgr();
	virtual ~LoginMgr();

	void enum_world_servers(std::vector<LoginSrvInfo>& info);

	void add_world_server(LoginSocketBase* s);
	void remove_world_server(LoginSocketBase* s);

	void update_world_server(LoginSocketBase* s, LoginSrvInfo& info);
	void update_world_server_name(LoginSocketBase* s, std::string name);
	void update_world_server_info(LoginSocketBase* s, std::string info);
	void update_world_server_status(LoginSocketBase* s, LoginSrvStatus status);

	void add_client(LoginSocketBase* s);
	void remove_client(LoginSocketBase* s);

protected:
	std::map<SOCKET, LoginSocketBase*> _servers;
	std::map<LoginSocketBase*, LoginSrvInfo> _servers_info;
	std::map<SOCKET, LoginSocketBase*> _clients;
	Mutex _server_lock;
	Mutex _client_lock;
};

}


#endif
