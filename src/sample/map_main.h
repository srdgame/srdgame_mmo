#include "defs.h"
#include "fromdispatch.h"
#include "todispatch.h"

namespace ro
{
size_t map_from_stream(Packet* p, const char* buf, size_t size)
{
	p->op = EC_NONE;
	size_t res = 0;
	if (size <= 2)
	{
		return res;
	}
	else
	{
		uint16 opcode = PUINT16(buf, 0);
		FromStruct s(p, buf, size);
		return FromDispatch::get_singleton().call(opcode, s);
	}
}

size_t map_to_stream(char* buf, const Packet* p)
{
	ToStruct s(buf, p);
	return ToDispatch::get_singleton().call(p->op, s);
}

}
