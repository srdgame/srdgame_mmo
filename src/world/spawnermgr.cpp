#include "spawnermgr.h"
#include "metadefs.h"
#include <list>
#include <string>
#include <cassert>
#include "autolock.h"
#include "conversion_cast.h"

using namespace srdgame;
using namespace std;

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
		return;

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

		if (buf[i] == '[')
		{
			SMF_Node node;
			int j = buf.find(i, ']');
			if (j == buf.npos)
				continue;

			node._index = conversion_cast<unsigned int>(buf.substr(i, j));
			if (read_node(file, node))
			{
				// load spawner.
				int id = 0 - node._index;
				if(_smf_nodes.find(id) == _smf_nodes.end())
				{
					Spawner* s = new Spawner(id);	
					s->load(node._file_name);
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
	}
}

bool SpawnerMgr::read_node(ifstream& file, SMF_Node& node)
{
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
		}
		else if (buf.substr(0, i).find("file") != buf.npos)
		{
			++i;
			while ((buf[i] == ' ' || buf[i] == '\t') && i < buf.size())
				++i;

			if (i == buf.size())
				continue;
			node._file_name = buf.substr(i);
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
	_lock.lock();
	std::list<Spawner*>::iterator ptr = _spawners.begin();
	for (; ptr != _spawners.end(); ++ptr)
	{
		delete (*ptr);
	}
	_spawners.clear();
	_smf_nodes.clear();
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


