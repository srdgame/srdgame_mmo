#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "rounit.h"
#include "rocharinfo.h"

using ro::RoPosition;

#include <string>

namespace srdgame
{
struct SpawnerInfo
{
	int _unit_id;
	std::string _map;
	long _time;
};
class Spawner : public RoUnit
{
	class SpawnerMgr;
protected:
	Spawner(const SpawnerInfo& info, const RoPosition& pos);
	~Spawner();
	
	long notify(long time);

	bool save(const char* root_dir);
	bool load(const char* fn);
private:
	SpawnerInfo _info;
};
}

#endif
