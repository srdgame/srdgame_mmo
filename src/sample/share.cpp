#include "share.h"
#include "ro_defs.h"
#include <cstring>

using namespace srdgame;

namespace ro
{
size_t send_account(char* buf, const Packet* packet)
{
	PUINT32(buf, 0) = packet->param.Int;
	return 4;
}
size_t send_6a_to_disconnect(char* buf, uint8 reason)
{
	memset(buf, 0, 23);
	PINT16(buf, 0) = 0x6a;
	PINT8(buf, 2) = reason;
	return 23;
}

//size_t to_send_0x006a(char* buf, const Packet* packet)
TO_DC(0x006a)
{
	return send_6a_to_disconnect(buf, packet->param.Char);
}

TO_DC(0x0283)
{
	PUINT16(buf, 0) = 0x283;
	PUINT32(buf, 2) = packet->param.Int;
	return 6;
}

TO_DC(0x0073)
{
	MapAuthOK* ok = (MapAuthOK*)packet->param.Data;
	PUINT16(buf, 0) = 0x0073;
	PUINT32(buf, 2) = ok->_tick_count;
	SET_POS((uint8*)buf, 6, ok->_x, ok->_y, ok->_dir);
	PUINT8(buf, 9) = 5;
	PUINT8(buf, 10) = 5;
	return 11;
}

}

