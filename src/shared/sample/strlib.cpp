#include "strlib.h"

bool bin2hex(char* out, unsigned char* in, size_t size)
{
	char toHex[] = "0123456789abcdef";
	size_t i;

	for( i = 0; i < size; ++i )
	{
		*out++ = toHex[(*in & 0xF0) >> 4];
		*out++ = toHex[(*in & 0x0F) >> 0];
		++in;
	}
	*out = '\0';
	return true;
}
