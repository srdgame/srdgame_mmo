#include "packetparser.h"
#include <cstring>
#include <dlfcn.h>

#undef PACKET_DEBUG

#include "log.h"

using namespace srdgame;

static int g_header_len = sizeof(int) + sizeof(int);
static int g_packet_param_len = sizeof(PacketParam);

void PacketParser::free(Packet& p)
{
	int param_len = p.len - g_header_len;
	if (param_len <= 0)
		return;
	if (param_len > g_packet_param_len)
	{
		delete[] p.param.Data;
	}
	return;
}

int PacketParser::get_ex_len(const Packet& p)
{
	//LogDebug("PacketParser", "p.len = %d, g_header_len = %d, so get_ex_len = %d", p.len, g_header_len, p.len - g_header_len);
	return p.len - g_header_len - g_packet_param_len;
}

PacketParser::~PacketParser()
{
	if (_lib != NULL)
	{
		dlclose(_lib);
	}
}
size_t PacketParser::from_inter(Packet& dest, const char* src, size_t size)
{
	// Check whether the data is really big to convert.
	if (size < g_header_len)
		return 0;

	::memcpy((char*)&dest, src, g_header_len);
	int param_len = dest.len - g_header_len;

	if (dest.len >= MAX_PACKET_LEN)
	{
		LogError("PacketParser", "The packet is invalid, because it is larger than MAX. Size: %d", dest.len);
		return 0;
	}

	// Check whether the data is really big to convert.
	if (size < dest.len)
	{
#ifdef PACKET_DEBUG
		LogDebug("PacketParser", "Data is not enough for current packet which size : %d", dest.len);
#endif
		return 0;
	}
#ifdef PACKET_DEBUG
	LogDebug("PacketParser", "param_len is : %d", param_len);
#endif
	// invalid stream.
	if (param_len < g_packet_param_len)
	{

		LogError("PacketParser", "Invalide stream found");
#ifdef PACKET_DEBUG
		char sz[size + 1];
		::memset(sz, 0, size + 1);
		::memcpy(sz, src, size);
		LogDebug("PacketParser", "Invalid stream is : %s", sz);
#endif
		return 0;
	}

	// if the param needs to be pointer or not
	if (param_len > g_packet_param_len)
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
#ifdef PACKET_DEBUG
	LogDebug("PacketParser", "Packet:\t opcode: %d\t len: %d", src.op, src.len);
	LogDebug("PacketParser", "g_header_len is %d", g_header_len);
	LogDebug("PacketParser", "g_packet_param_len is %d", g_packet_param_len);
	LogDebug("PacketParser", "sizeof(PacketParam) is %d", sizeof(PacketParam));
#endif
	// copy the header.
	::memcpy(dest, (char*)&src, g_header_len);
	
	// param
	int param_len = src.len - g_header_len;
#ifdef PACKET_DEBUG
	LogDebug("PacketParser", "Packet param len is %d", param_len);
#endif
	if (param_len > g_packet_param_len)
	{
		::memcpy(dest + g_header_len, (char*)src.param.Data, param_len);
	}
	else
	{
		::memcpy(dest + g_header_len, (char*)&src.param.Long, param_len);
	}
	return src.len;
}

bool PacketParser::init_ex(const char* ex_name)
{
	if (_lib != NULL)
	{
		dlclose(_lib);
	}
	_lib = dlopen(ex_name, RTLD_LAZY);
	if (_lib == NULL)
	{
		LogError("PacketParser", "Could not load protocol lib : %s  The error is : %s", dlerror());
		return false;
	}
	init_func f = (init_func)(dlsym(_lib, "init"));
	char* error = dlerror();
	if (NULL != error)
	{		
		LogError("PacketParser", "Could not find init(), error is : %s", error);
		dlclose(_lib);
		_lib = NULL;
		return false;
	}
	_ex_from_func = (from_stream_func)dlsym(_lib, "from_stream");
	error = dlerror();
	if (NULL != error)
	{
		LogError("PacketParser", "Could not find from_stream, error is : %s", error);
		dlclose(_lib);
		_lib = NULL;
		return false;
	}
	_ex_to_func = (to_stream_func)dlsym(_lib, "to_stream");
	error = dlerror();
	if (NULL != error)
	{
		LogError("PacketParser", "Could not find to_stream, error is : %s", error);
		dlclose(_lib);
		_lib = NULL;
		return false;
	}
	LogSuccess("PacketParser", "Load external protocol parser successfully!!!");

	// try to init the extern parser.
	(*f)();

	Packet p;
	char* ch = NULL;
	if (0 == from_ex(p, ch, 0))//&& 0 == to_ex(ch, p))
	{
		LogSuccess("PacketParser", "Checking OK");
	}
	else
	{
		LogError("PacketParser", "Checking Failed");
		return false;
	}
	return true;
}

size_t PacketParser::from_ex(Packet& dest, const char* src, size_t size)
{
	return (*_ex_from_func)(&dest, src, size);
}

size_t PacketParser::to_ex(char* dest, const Packet& src)
{	
	return (*_ex_to_func)(dest, &src);
}
