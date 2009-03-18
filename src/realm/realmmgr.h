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
class RealmSocketBase;
class RealmMgr : public Singleton < RealmMgr >
{
public:
	RealmMgr();
	virtual ~RealmMgr();

	void enum_world_servers(std::vector<WorldSrvInfo>& info);

	void add_world_server(RealmSocketBase* s);
	void remove_world_server(RealmSocketBase* s);

	void update_world_server(RealmSocketBase* s, WorldSrvInfo& info);
	void update_world_server_name(RealmSocketBase* s, std::string name);
	void update_world_server_status(RealmSocketBase* s, WorldSrvStatus status);

	void add_client(RealmSocketBase* s);
	void remove_client(RealmSocketBase* s);

	// realm server attributes.
	void set_name(std::string name);
	std::string get_name();
	void set_info(RealmSrvInfo& info);
	RealmSrvInfo& get_info();

protected:
	std::map<SOCKET, RealmSocketBase*> _servers;
	std::map<RealmSocketBase*, WorldSrvInfo> _servers_info;
	std::map<SOCKET, RealmSocketBase*> _clients;
	Mutex _server_lock;
	Mutex _client_lock;

	Mutex _lock;
	RealmSrvInfo _info;
};

}


#endif
