#ifndef UPDATER_H_
#define UPDATER_H_

#include "nobject.h"

namespace srdgame
{
class Updater
{
public:
	virtual ~Updater(){};

	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;

	virtual void add(NObject* obj) = 0;
	virtual void remove(NObject* obj) = 0;
};
}

#endif
