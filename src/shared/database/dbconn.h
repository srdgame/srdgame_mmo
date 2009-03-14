#ifndef DATABASE_CONNECTION_H_
#define DATABASE_CONNECTION_H_

#include "mutex.h"

namespace srdgame
{
struct DBConn
{
	Mutex _lock; 
};
};

#endif
