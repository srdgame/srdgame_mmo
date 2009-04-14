#ifndef UPDATER_H_
#define UPDATER_H_

#include "waitor.h"

namespace srdgame
{
class Updater
{
public:
	virtual ~Updater(){};

	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;

	virtual void add(Waitor* w) = 0;
	virtual void remove(Waitor* w) = 0;
};
}

#endif
