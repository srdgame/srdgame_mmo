#include "strlib.h"

bool bin2hex(char* out, unsigned char* in, size_t size)
{
	char toHex[] = "0123456789abcdef";
	size_t i;

	for( i = 0; i < count; ++i )
	{
		*output++ = toHex[(*input & 0xF0) >> 4];
		*output++ = toHex[(*input & 0x0F) >> 0];
		++input;
	}
	*output = '\0';
	return true;
}
