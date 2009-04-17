#include "unitmgr.h"

using namespace srdgame;
using namespace std;

UnitMgr::UnitMgr()
{
}

UnitMgr::~UnitMgr()
{

}

int UnitMgr::get_count()
{
	return _units.size();
}

void UnitMgr::add(RoUnit* unit)
{
	_units.insert(make_pair<int, RoUnit*>(unit->get_id(), unit));
}

void UnitMgr::remove(RoUnit* unit)
{
	_units.erase(unit->get_id());
}

