#ifndef PACKET_DEFS_H_
#define PACKET_DEFS_H_

#include <cstddef>
#include "metadefs.h"

//#include "packetparser.h"

namespace srdgame
{

enum InterOpcode
{
	I_PING = 0, // Just ping. Both side command.  Normally server will ping client per 5 seconds?
	I_OFFLINE = 1, // Say goodbye, both side command.
	I_NOTIFY = 2, // Notify others we are going online. Both side action and its param is the client type: Login = 1, World = 2, Realm = 0,
	
	IS_GET_NAME = 31, // Ask for name of client.
	IC_NAME = 32, // send back the name in one string
	
	IS_GET_STATUS = 40, //
	IC_STATUS = 41, // 
	IC_POST_STATUS = 42, // Post the status to server

	IS_GET_INFO = 50, // ask for whole information, include Name and Status and any other information.
	IC_INFO = 51, //

	// More
};

union PacketParam
{
	const char* Data;
	int Int;
	long Long;
	float Float;
	short Short;
	bool Bool;
	char Char;
};

#define MAX_PACKET_LEN 1024 // we do believen this should be long enough for any packet...

// TODO: should I consider about size of int?  So far let me just simply assume it is 32.
struct Packet
{
	long len; // the length of whole packet.
	long id; // the packet id/index ?
	long op; // opcode of this packet.
	PacketParam param; // the param point or the value of param (if it just one int or less than int.

};

}

#define PCHAR(buf, pos) (buf + (pos))

#define PUINT8(buf, pos) (*(uint8*)PCHAR(buf, pos))
#define PUINT16(buf, pos) (*(uint16*)PCHAR(buf, pos))
#define PUINT32(buf, pos) (*(uint32*)PCHAR(buf, pos))
#define PUINT64(buf, pos) (*(uint64*)PCHAR(buf, pos))

#define PINT8(buf, pos) (*(int8*)PCHAR(buf, pos))
#define PINT16(buf, pos) (*(int16*)PCHAR(buf, pos))
#define PINT32(buf, pos) (*(int32*)PCHAR(buf, pos))
#define PINT64(buf, pos) (*(int64*)PCHAR(buf, pos))


#endif
