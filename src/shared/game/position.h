#ifndef POSITION_H_
#define POSITION_H_

#include "metadefs.h"


struct Position
{
public:
	uint32 x;
	uint32 y;
	uint32 z;
	uint32 r;
};

template<class T>
struct PositionEx : public Position
{
	T* _ex;
};

#endif
