#ifndef MAP_MGR_H_
#define MAP_MGR_H_

#include <singleton.h>
#include "mutex.h"
#include <map>
#include <string>
#include "romap.h"

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
	void unload_maps();
protected:
	std::map<int, Map*> _maps;
	std::map<std::string, int> _map_ids;
	// holder of all the memorys.
	std::vector<ro::MapData> _maps_data;
	InterSocket* _inter_socket;
	Mutex _lock;
};
}

#endif
