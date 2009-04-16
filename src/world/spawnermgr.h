#ifndef SPAWNER_MGR_H_
#define SPAWNER_MGR_H_

#include "singleton.h"
#include <list>
#include <string>

// Only used to maintains the spawners.  Load and save them.
namespace srdgame
{
class SpawnerMgr : public Singleton < SpanwerMgr >
{
public:
	SpawnerMgr();
	~SpawnerMgr();

	void load(const std::string& root_dir, const std::string& index_fn);
	void unload();
	void save();
	void refresh(int id);

	void add(const SpawnInfo& info);
protected:

	bool _inited;
	std::string _root_dir;
	std::string _index_fn;
	std::list<Spawner*> _spawners;
	Mutex _lock;
};
};

#endif
