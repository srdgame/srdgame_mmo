#ifndef POSITION_H_
#define POSITION_H_

#include "metadefs.h"

namespace srdgame
{
struct Position
{
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
