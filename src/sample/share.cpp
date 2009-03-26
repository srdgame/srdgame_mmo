#include "share.h"
#include <cstring>

namespace ro
{
size_t send_6a_to_disconnect(char* buf, uint8 reason)
{
	memset(buf, 0, 23);
	PINT16(buf, 0) = 0x6a;
	PINT8(buf, 2) = reason;
	return 23;
}














}
