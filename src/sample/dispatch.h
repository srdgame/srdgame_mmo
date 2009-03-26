#ifndef DISPATCH_H_
#define DISPATCH_H_

namespace srdgame
{
struct Packet;
}
namespace ro
{
typedef size_t (*From)(Packet* packet, const char* src, size_t size);
typedef size_t (*To)(char* buf, const Packet* packet);
class Dispatch
{
	static From[0x0500];
	static To[0x0500];

public:
	inline static size_t dispatch_from(Packet* packet, const char* src, size_t size)
	{

	}
	inline static size_t dispatch_to(char* buf, const Packet* packet);

	inline static void add_from(uint16, From& ptr)
	{
	}
	inline static void add_to(uint16, To& ptr)
	{
	}
	inline static size_t _from(Packet* packet, const char* src, size_t size)
	{
		return size;
	}
	inline static size_t _to(char* buf, const Packet* packet)
	{
		return 0;
	}
};
}

#endif
