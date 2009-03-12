#ifndef PACKET_PARSER_H_
#define PACKET_PARSER_H_

#include "packetdefs.h"
#include "singleton.h"

#include <cstddef>

namespace srdgame
{
typedef size_t (*from_stream_func)(Packet* dest, const char* src, size_t size);
typedef size_t (*to_stream_func)(char* dest, const Packet* src);

class PacketParser : public Singleton < PacketParser >
{
public:
	~PacketParser();
	// return converted bytes count.
	size_t from_inter(Packet& dest, const char* src, size_t size);
	// to_inter will return the dest stream length.
	size_t to_inter(char* dest, const Packet& src);

	bool init_ex(const char* ex_name); // later we will use plugins to do the parser...

	// return same as from_inter
	size_t from_ex(Packet& dest, const char* src, size_t size);
	// return as to_inter.
	size_t to_ex(char* dest, const Packet& src);

public:
	static void free(Packet& p);
	static int get_ex_len(Packet& p);
private:
	from_stream_func _ex_from_func;
	to_stream_func _ex_to_func;
	void* _lib;
};
}

#endif
