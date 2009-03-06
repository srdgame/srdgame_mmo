#include "packetparser.h"
#include <cstring>

using namespace srdgame;

static int g_header_len = sizeof(int) + sizeof(int) + sizeof(int);
static int g_void_ptr_len = sizeof(void*);

size_t PacketParser::from_inter(Packet& dest, const char* src)
{
	::memcpy((char*)&dest, src, g_header_len);
	int param_len = dest.len - g_header_len;

	// invalid stream.
	if (param_len < 0)
		return 0;

	// if the param needs to be pointer or not
	if (param_len > g_void_ptr_len)
	{
		char* buf = new char[param_len];
		::memcpy(buf, src + g_header_len, param_len);
		dest.param.Data = buf; // outside of here needs to handle the deletes.
	}
	else
	{
		dest.param.Long = 0L;
		::memcpy((char*)&dest.param.Long, src + g_header_len, param_len);
	}

	return dest.len;
}

size_t PacketParser::to_inter(char* dest, const Packet& src)
{
	// invalid packet.
	if (src.len < g_header_len)
		return 0;

	// copy the header.
	::memcpy(dest, (char*)&src, g_header_len);
	
	// param
	int param_len = src.len - g_header_len;
	if (param_len > g_void_ptr_len)
	{
		::memcpy(dest + g_header_len, (char*)src.param.Data, param_len);
	}
	else
	{
		::memcpy(dest + g_header_len, (char*)&src.param.Long, param_len);
	}
	return src.len;
}

bool init_ex(char* ex_name)
{
	return false;
}

bool from_ex(Packet& dest, const char* src)
{
	return false;
}

size_t to_ex(char* src, const Packet& dest)
{
	return 0;
}
