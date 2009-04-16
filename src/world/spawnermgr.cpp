#include "spawnermgr.h"
#include "metadefs.h"
#include <fstream>
#include <list>
#include <string>

using namespace srdgame;
using namespace std;

struct SMF_Header
{
	int _count;
	string _author;
	string _version;
};

struct SMF_Node
{
	int _index;
	string _file_name;
};

SpawnerMgr::SpawnerMgr() : _inited(false)
{
}

SpawnerMgr::~SpawnerMgr()
{
	// you must unload it mannualy
	assert(_inited == false);
}

void SpawnerMgr::load(const string& roo_dir, const string& index_fn)
{
	AutoLock lock(_lock);
	if (_inited)
		return;

	_inited = true;

}

void SpawnerMgr::unload()
{
}

void SpawnerMgr::save()
{
}
void SpawnerMgr::refresh(int id)
{
}

void SpawnerMgr::add(const SpawnInfo& info)
{
}


