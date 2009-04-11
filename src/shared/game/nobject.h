#ifndef N_OBJECT_H_
#define N_OBJECT_H_

#include "object.h"

namespace srdgame
{
enum NPriority
{
	NP_NOTHING = 0, // Even you do not notify, it still ok.
	NP_CAN_IGNORE, // Even you do not notify, it still ok for a while, but not always.
	NP_MAY_GOOD, // You may notify it, and it makes good stuff.
	NP_HAVE_TO, // You have to notify it when you get time.
	NP_NEEDED, // This notify is needed by system.
	NP_GOD, // Ok this is god, take him first.
	NP_COUNT,
};

class NObject : public Object
{
public:
	virtual ~NObject(){}

public:
	// Notified, return next needed notify time.  Normally this will be checked just as prefer not what will exactly happen.
	virtual long notify(long time) = 0;
	virtual NPriority get_priority() {return NP_NOTHING;};
};
}
#endif
