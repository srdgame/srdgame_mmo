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
class RealmSocket;
class RealmMgr : public Singleton < RealmMgr >
{
public:
	RealmMgr();
	virtual ~RealmMgr();

	std::vector<LoginSrvInfo> enum_login_servers();

	void add_login_server(RealmSocket* s);
	void remove_login_server(RealmSocket* s);

	void update_login_server(RealmSocket* s, LoginSrvInfo& info);

	void add_client(RealmSocket* s);
	void remove_client(RealmSocket* s);

protected:
	std::map<SOCKET, RealmSocket*> _servers;
	std::map<RealmSocket*, LoginSrvInfo> _servers_info;
	std::map<SOCKET, RealmSocket*> _clients;
	Mutex _server_lock;
	Mutex _client_lock;
};

}


#endif
