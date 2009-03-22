#ifndef POSITION_H_
#define POSITION_H_

#include "metadefs.h"

namespace srdgame
{
struct Position
{
	Position() : _x (0), _y(0), _z(0), _r(0)
	{
	}
	int _x;
	int _y;
	int _z;
	int _r;
};

template<class T>
struct PositionEx : public Position
{
	T* _ex;
};
}
#endif
