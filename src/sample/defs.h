#ifndef _DEFS_H_
#define _DEFS_H_

#include "packetdefs.h"
#include "opcode.h"

using namespace srdgame;
using namespace srdgame::opcode;

extern "C"
{
extern size_t from_stream(Packet* dest, const char* src, size_t size);
extern size_t to_stream(char* dest, const Packet* src);
}
enum {
	SEX_FEMALE = 0,
	SEX_MALE,
	SEX_SERVER
};

#endif
