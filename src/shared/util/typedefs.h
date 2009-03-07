#ifndef SRDGAME_TYPE_DEFS_H_
#define SRDGAME_TYPE_DEFS_H_

// Shared structs between servers.
//

#include <string>

enum LoginSrvStatus
{
	SS_OFFLINE = 0,
	SS_READY = 1,
	SS_FULL = 2,
	SS_CLOSING = 4,
	SS_TESTING = 128,
};
struct LoginSrvInfo
{
	std::string name;
	LoginSrvStatus status;
	std::string info;
};

#endif
