#ifndef TIME_DEFS_H_
#define TIME_DEFS_H_

#include <ctime>
#include <sys/time.h>

namespace srdgame
{
/// platform-abstracted tick retrieval
inline unsigned int tick(void)
{
#if defined(WIN32)
	return GetTickCount();
#elif (defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0 && defined(_POSIX_MONOTONIC_CLOCK) /* posix compliant */) || (defined(__FreeBSD_cc_version) && __FreeBSD_cc_version >= 500005 /* FreeBSD >= 5.1.0 */)
	struct timespec tval;
	clock_gettime(CLOCK_MONOTONIC, &tval);
	return tval.tv_sec * 1000 + tval.tv_nsec / 1000000;
#else
	struct timeval tval;
	gettimeofday(&tval, NULL);
	return tval.tv_sec * 1000 + tval.tv_usec / 1000;
#endif
}

inline unsigned int gettick(void)
{
	return tick();
}

}

#endif
