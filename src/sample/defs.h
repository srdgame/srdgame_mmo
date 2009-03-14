#ifndef _DEFS_H_
#define _DEFS_H_

#include "packetdefs.h"
#include "typedefs.h"

using namespace srdgame;

extern "C"
{
extern size_t from_stream(Packet* dest, const char* src, size_t size);
extern size_t to_stream(char* dest, const Packet* src);
}

#define PCHAR(buf, pos) (buf + (pos))

#define PUINT8(buf, pos) (*(uint8*)CHAR(buf, pos))
#define PUINT16(buf, pos) (*(uint16*)CHAR(buf, pos))
#define PUINT32(buf, pos) (*(uint32*)CHAR(buf, pos))
#define PUINT64(buf, pos) (*(uint64*)CHAR(buf, pos))

#endif
