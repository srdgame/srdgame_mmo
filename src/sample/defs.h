#ifndef _DEFS_H_
#define _DEFS_H_

#include "packetdefs.h"

using namespace srdgame;

extern "C"
{
extern size_t from_stream(Packet* dest, const char* src, size_t size);
extern size_t to_stream(char* dest, const Packet* src);
}

#endif
