#include "spawnermgr.h"
#include "metadefs.h"
#include <list>
#include <string>
#include <cassert>
#include "autolock.h"
#include "conversion_cast.h"
#include "log.h"
#include "worldmgr.h"
#include "mapmgr.h"
#include "map.h"
#include "updater.h"

using namespace srdgame;
using namespace std;

#define LN "SpawnMGR"

SpawnerMgr::SpawnerMgr() : _inited(false)
{
}

SpawnerMgr::~SpawnerMgr()
{

	// you must unload it mannualy
	assert(_inited == false);
}

void SpawnerMgr::load(const string& root_dir, const string& index_fn)
{
	AutoLock lock(_lock);
	if (_inited)
		return;

	_root_dir = root_dir;
	_index_fn = index_fn;

	ifstream file(index_fn.c_str());

	if (!file.good())
	{
		LogError(LN, "Could not open spaners conf : %s", index_fn.c_str());
		return;
	}

	_inited = true;

	string buf;
	while (getline(file, buf))
	{
		if (buf.empty())
			continue; // empty line.
		int i = 0;
		
		while ((buf[i] == ' ' || buf[i] == '\t')&& i < buf.size())
		{
			++i;
		}

		if (i == buf.size())
			continue; // only has space.

		if (buf[i] == '#')
			continue; //commented.

		LogDebug(LN, "Processing %s", buf.c_str());
		LogDebug(LN, "start from : %d", i);
		if (buf[i] == '[')
		{
			SMF_Node node;
			int j = buf.find(']', i);
			if (j == buf.npos)
			{
				LogDebug(LN, "Can not found ]");
				continue;
			}

			node._index = conversion_cast<unsigned int>(buf.substr(i + 1, j));
			if (read_node(file, node))
			{
				// load spawner.
				int id = 0 - node._index;
				if(_smf_nodes.find(id) == _smf_nodes.end())
				{
					Spawner* s = new Spawner(id);
					string path = root_dir + "/";
					path += node._file_name;
					s->load(path);
					// TODO:
					_spawners.push_back(s);
					_smf_nodes.insert(make_pair<int, SMF_Node>(id, node));
				}
				else
				{
					//TODO:
				}
			}
		}
		else
		{
			LogError(LN, "Incorrect formate found!!");
		}
	}
	Updater* updater = WorldMgr::get_singleton().get_updater();
	MapMgr* map_mgr = MapMgr::get_instance();
	for (std::list<Spawner*>::iterator ptr = _spawners.begin(); ptr != _spawners.end(); ++ptr)
	{
		Map* map = map_mgr->get_map((*ptr)->get_spawn_info()._map);
		if (!map)
			continue;
		map->add_unit(*ptr);
		updater->add(*ptr);
	}
	LogSuccess(LN, "Loaded spawner conf successful, count : %u", _smf_nodes.size());
}

bool SpawnerMgr::read_node(ifstream& file, SMF_Node& node)
{
	LogDebug(LN, "Loading one node [index: %d]", node._index);
	node._name = "";
	node._file_name = "";
	string buf;
	while (getline(file, buf))
	{
		if (buf.empty())
			break; // empty line.
		int i = 0;
		
		while ((buf[i] == ' ' || buf[i] == '\t')&& i < buf.size())
		{
			++i;
		}

		if (i == buf.size())
			break; // only has space.

		if (buf[i] == '#')
			continue; //commented.

		// 
		i = buf.find("=");
		if (i == buf.npos)
			continue;

		if (buf.substr(0, i).find("name") != buf.npos)
		{
			i++;
			while ((buf[i] == ' ' || buf[i] == '\t' ) && i < buf.size())
			{
				++i;
			}
			if (i == buf.size())
				continue;
			node._name = buf.substr(i);
			LogDebug(LN, "Get node name : %s", node._name.c_str());
		}
		else if (buf.substr(0, i).find("file") != buf.npos)
		{
			++i;
			while ((buf[i] == ' ' || buf[i] == '\t') && i < buf.size())
				++i;

			if (i == buf.size())
				continue;
			node._file_name = buf.substr(i);
			LogDebug(LN, "Get spawner file name : %s", node._file_name.c_str());
		}
		else
		{
			//TODO:
		}
	}
	return !(node._file_name.empty());
}

void SpawnerMgr::unload()
{
	LogError(LN, "Unloading the spawner conf");
	_lock.lock();
	std::list<Spawner*>::iterator ptr = _spawners.begin();
	Updater* updater = WorldMgr::get_singleton().get_updater();
	MapMgr* map_mgr = MapMgr::get_instance();
	for (; ptr != _spawners.end(); ++ptr)
	{
		Map* map = (*ptr)->get_map();//map_mgr->get_map((*ptr)->get_spawn_info()._map);
		if (map)
		{
			map->remove_unit(*ptr);
		}
		updater->remove(*ptr);
		delete (*ptr);
	}
	_spawners.clear();
	_smf_nodes.clear();
	_inited = false;
	_lock.unlock();
}

void SpawnerMgr::save()
{
	ofstream file (_index_fn.c_str());
	if (file.bad())
		return;

	AutoLock lock(_lock);
	std::map<int, SMF_Node>::iterator ptr = _smf_nodes.begin();
	for (; ptr != _smf_nodes.end(); ++ptr)
	{
		file << endl;
		file << "[" << ptr->second._index << "]" << endl;
		file << "name = " << ptr->second._name << endl;
		file << "file = " << ptr->second._file_name << endl;
		file << endl;
	}
	// TODO: Need to save the spawner information.
}
void SpawnerMgr::refresh(int id)
{
	//TODO:
}

void SpawnerMgr::add(const SpawnerInfo& info)
{
//TODO:
}


