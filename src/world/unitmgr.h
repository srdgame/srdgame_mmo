#ifndef UNIT_MGR_H_
#define UNIT_MGR_H_

#include "rounit.h"
#include <map>

namespace srdgame
{
class UnitMgr
{
public:
	UnitMgr();
	~UnitMgr();

	int get_count();
	
	void add(RoUnit* unit);
	void remove(RoUnit* unit);

protected:
	std::map<int, RoUnit*> _units;	
};
}

#endif

