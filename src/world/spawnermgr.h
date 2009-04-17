#ifndef SPAWNER_MGR_H_
#define SPAWNER_MGR_H_

#include "singleton.h"
#include <list>
#include <map>
#include <string>
#include <fstream>
#include "spawner.h"
#include "mutex.h"

// Only used to maintains the spawners.  Load and save them.
namespace srdgame
{
class SpawnerMgr : public Singleton < SpawnerMgr >
{
struct SMF_Node
{
	int _index;
	std::string _name;
	std::string _file_name;
};
public:
	SpawnerMgr();
	~SpawnerMgr();

	void load(const std::string& root_dir, const std::string& index_fn);
	void unload();
	void save();
	void refresh(int id);

	void add(const SpawnerInfo& info);

protected:
	bool read_node(std::ifstream& file, SMF_Node& node);
protected:

	bool _inited;
	std::string _root_dir;
	std::string _index_fn;
	std::list<Spawner*> _spawners;
	std::map<int, SMF_Node> _smf_nodes;
	Mutex _lock;
};
};

#endif
