#include "unitmgr.h"
#include "log.h"

using namespace srdgame;
using namespace std;

UnitMgr::UnitMgr()
{
}

UnitMgr::~UnitMgr()
{
	// To Clean up the units.
	map<int, RoUnit*>::iterator ptr = _units.begin();
	for (; ptr != _units.end(); ++ptr)
	{
		if (ptr->second)
		{
			delete ptr->second;
		}
	}
	_units.clear();
}

int UnitMgr::get_count()
{
	return _units.size();
}

void UnitMgr::add(RoUnit* unit)
{
	LogDebug("UnitMgr", "Adding one unit id : %d", unit->get_id());
	_units.insert(make_pair<int, RoUnit*>(unit->get_id(), unit));
}

void UnitMgr::remove(RoUnit* unit)
{
	_units.erase(unit->get_id());
}
void UnitMgr::get_all(std::vector<RoUnit*>& units)
{
	units.clear();
	units.resize(_units.size());
	map<int, RoUnit*>::iterator ptr = _units.begin();
	for (; ptr != _units.end(); ++ptr)
	{
		if (ptr->second)
		{
			units.push_back(ptr->second);
		}
	}
}

