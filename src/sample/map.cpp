#include "map.h"
#include "share.h"

namespace ro
{
size_t from_map_connect(Packet* dest, const char* src, size_t size)
{
	// support only the lastest client.
	uint16 op = PUINT16(src, 0);
	int index[5];
	size_t len = 0;
	switch (op)
	{
		case 0x0436:
			index[0] = 2;
			index[1] = 6;
			index[2] = 10;
			index[3] = 14;
			index[4] = 18;
			len = 19;
			break;
		case 0x009b:
			index[0] = 4;
			index[1] = 9;
			index[2] = 17;
			index[3] = 18;
			index[4] = 25;
			len = 26;
			break;
		default:
			return 0;
			break;
	}

	ConnectToMap* ctm = new ConnectToMap();
	ctm->_ver = op;
	ctm->_account_id = PUINT32(src, index[0]);
	ctm->_char_id = PUINT32(src, index[1]);
	ctm->_login_id11 = PUINT32(src, index[2]);
	ctm->_client_tick = PUINT32(src, index[3]);
	ctm->_sex = PUINT8(src, index[4]);

	dest->op = EC_CONNECT_TO_MAP;
	dest->len = sizeof(Packet) + sizeof(ConnectToMap);
	dest->param.Data = (char*)ctm;

	return len;
}
size_t to_map_connect(char* buf, const Packet* packet)
{
	if (packet->param.Char)
	{
		return send_6a_to_disconnect(buf, (uint8)packet->param.Char);
	}
	else
	{
		PUINT(src, 0) = 0x0283;
		PUINT(src, 2) = packet
	}
}
