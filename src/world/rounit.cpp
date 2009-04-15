#include "rounit.h"

using namespace srdgame;

RoUnit::RoUnit(int id) : Unit(id), _pri(NP_NOTHING), _time(0), _by(NULL)
{
}

RoUnit::~RoUnit()
{
}

long RoUnit::notify(long time)
{
	_time = time;
	return _time + _pri * 1000;
}

bool RoUnit::move_to(Position* pos, Object* by)
{
	_pos._x = pos->_x;
	_pos._y = pos->_y;
	_pos._z = pos->_z;
	_pos._r = pos->_r;
	_by = by;
	return true;
}

bool RoUnit::clicked(Object* by)
{
	_by = by;
	return true;
}

bool RoUnit::attacked(Object* by)
{
	_by = by;
	return true;
}


