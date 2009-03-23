#ifndef MAP_MGR_H_
#define MAP_MGR_H_

#include <singleton.h>
#include "mutex.h"
#include <map>
#include <string>

namespace srdgame
{
class InterSocket;
class Map;
class MapMgr : public Singleton < MapMgr >
{
public:
	MapMgr();
	~MapMgr();
public:
	void load_maps();
	void bind(InterSocket* socket);
	void send_maps();
protected:
	std::map<int, Map*> _maps;
	std::map<std::string, int> _map_ids;
	InterSocket* _inter_socket;
	Mutex _lock;
};
}

#endif
