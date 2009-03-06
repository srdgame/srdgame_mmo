#ifndef PACKET_DEFS_H_
#define PACKET_DEFS_H_

namespace srdgame
{

enum InterOpcode
{
	I_PING = 0, // Just ping. Both side command.  Normally server will ping client per 5 seconds?
	I_OFFLINE = 1, // Say goodbye, both side command.
	I_NOTIFY = 2, // Notify others we are going online. Both side action and its param is the client type: Login = 1, world = 2, Realm = 0,
	
	IS_GET_NAME = 31, // Ask for name of client.
	IC_NAME = 32, // send back the name in one string
	
	IS_GET_STATUS = 40, //
	IC_STATUS = 41, // 
	IC_POST_STATUS = 42, // Post the status to server

	IS_GET_INFO = 50, // ask for more detail info, the reply structure is to be defined.
	IS_INFO = 51, //
	
	// More
};

union PacketParam
{
	char* Data;
	int Int;
	long Long;
	float Float;
	short Short;
	bool Bool;
};

// TODO: should I consider about size of int?  So far let me just simply assume it is 32.
struct Packet
{
	int len; // the length of whole packet.
	int id; // the packet id/index ?
	int op; // opcode of this packet.
	PacketParam param; // the param point or the value of param (if it just one int or less than int.
};

}

#endif
