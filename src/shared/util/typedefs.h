#ifndef SRDGAME_TYPE_DEFS_H_
#define SRDGAME_TYPE_DEFS_H_

// Shared structs between servers.
//

#include <string>
#include "metadefs.h"

enum RealmSrvStatus
{
	LS_OFFLINE = 0,
	LS_READY = 1,
	LS_FULL = 2,
	LS_CLOSING = 4,
	LS_TESTING = 128,
};
struct RealmSrvInfo
{
	char name[64];
	char ip[16];
	int port;
	RealmSrvStatus status;
	char info[256];
};

enum WorldSrvStatus
{
	WS_OFFLINE = 0,
	WS_READY = 1,
	WS_FULL = 2,
	WS_CLOSING = 4,
	WS_TESTING = 128,
};
enum WorldSrvType
{
	WT_NORMAL = 0,
	WT_PVP = 1,
	WT_TESTING = 128,
};

struct WorldSrvInfo
{
	char name[64];
	char ip[16];
	int port;
	WorldSrvStatus status;
	char info[256];
	WorldSrvType type;
};
#endif
