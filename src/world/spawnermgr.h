#ifndef SPAWNER_MGR_H_
#define SPAWNER_MGR_H_

#include "singleton.h"
#include <list>

// Only used to maintains the spawners.  Load and save them.
namespace srdgame
{
class SpawnerMgr : public Singleton < SpanwerMgr >
{
public:
	SpawnerMgr();
	~SpawnerMgr();

	void load(const char* root_dir, const char* index_fn);
	void unload();

	void add(const SpawnInfo& info);
protected:

	std::list<Spawner> _spawners;
};
};

#endif
