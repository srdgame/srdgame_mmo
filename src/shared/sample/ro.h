#include "ro_defs.h"
#include "roconf.h"
#include "rosql.h"
#include "rojob.h"
#include "rouser.h"
#include "strlib.h"

namespace ro
{
	// Call this function when you are trying to use any functions.
inline void init_ro()
{
	RoConf::get_singleton().load();
	RoJob::get_singleton().load();
}
}
