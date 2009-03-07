// This will mention all the data of realm 

#ifndef REALM_MANAGER_H_
#define REALM_MANAGER_H_

#include "singleton.h"
#include <vector>
#include <map>

#include "socketdefs.h"

namespace srdgame
{
struct ServerInfo
{
};
class RealmSocket;
class RealmMgr : public Singleton < RealmMgr >
{
public:
	RealmMgr();
	virtual ~RealmMgr();

	std::vector<ServerInfo> enum_login_servers();

	void add_login_server(RealmSocket* s);
	void remove_login_server(RealmSocket* s);

	void add_client(RealmSocket* s);
	void remove_client(RealmSocket* s);

protected:
	std::map<SOCKET, RealmSocket*> _servers;
	std::map<SOCKET, RealmSocket*> _clients;
};

}


#endif
