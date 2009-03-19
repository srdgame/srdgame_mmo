#ifndef N_OBJECT_H_
#define N_OBJECT_H_

enum NotifyPriority
{
	NP_NOTHING = 0,
	NP_CAN_IGNORE,
	NP_MAY_GOOD,
	NP_HAVE_TO,
	NP_NEEDED,
	NP_GOD,
};

class NObject
{
public:
	NObject();
	virtual NObject();

public:
	// Notified
	long notify(long time);
	NotifyPriority get_priority();
};

#endif
