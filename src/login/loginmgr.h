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

	void enum_world_servers(std::vector<WorldSrvInfo>& info);

	void add_world_server(LoginSocketBase* s);
	void remove_world_server(LoginSocketBase* s);

	void update_world_server(LoginSocketBase* s, WorldSrvInfo& info);
	void update_world_server_name(LoginSocketBase* s, std::string name);
	void update_world_server_info(LoginSocketBase* s, std::string info);
	void update_world_server_status(LoginSocketBase* s, WorldSrvStatus status);
	void update_world_server_type(LoginSocketBase* s, WorldSrvType type);

	void add_client(LoginSocketBase* s);
	void remove_client(LoginSocketBase* s);

	// login server attributes.
	void set_name(std::string name);
	std::string get_name();
	void set_info(std::string info);
	std::string get_info();

protected:
	std::map<SOCKET, LoginSocketBase*> _servers;
	std::map<LoginSocketBase*, WorldSrvInfo> _servers_info;
	std::map<SOCKET, LoginSocketBase*> _clients;
	Mutex _server_lock;
	Mutex _client_lock;

	Mutex _lock;
	std::string _name;
	std::string _info;
};

}


#endif
