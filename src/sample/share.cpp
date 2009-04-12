#include "share.h"
#include "ro_defs.h"
#include <cstring>
#include <cassert>
#include "rocharinfo.h"

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
// Send out the account_id to client, once we received one trying connect map server message.  Or you need to send out the 0x6a message to reject connection.
TO_DC(0x0283)
{
	PUINT16(buf, 0) = 0x283;
	PUINT32(buf, 2) = packet->param.Int;
	return 6;
}
// Send out map information that contains, where is the charater and its 朝向.
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
// send out one normal message to client.  Like chat info?
TO_DC(0x008e)
{
	if (packet->len == sizeof(Packet))
		return 0;

	RoMessage* msg = (RoMessage*)packet->param.Data;

	PUINT16(buf, 0) = 0x008e;
	//size_t len = strlen(packet->param.Data) + 1;
	//PUINT16(buf, 2) = 4 + len;
	//memcpy(PCHAR(buf, 4), packet->param.Data, len);
	//return 4 + len;

	if (msg->_id != 0)
	{
		PUINT16(buf, 2) = 4 + msg->_len + 4;
		PUINT32(buf, 4) = msg->_id;
		memcpy(PCHAR(buf, 8), msg->_msg, msg->_len);
		return 4 + 4 + msg->_len;
	}
	else
	{
		PUINT16(buf, 2) = 4 + msg->_len;
		memcpy(PCHAR(buf, 4), msg->_msg, msg->_len);
		return 4 + msg->_len;
	}
}
// Send out private message to client, that show in gray color.
TO_DC(0x017f)
{
	if (packet->len == sizeof(Packet))
		return 0;

	PUINT16(buf, 0) = 0x017f;
	size_t len = strlen(packet->param.Data) + 1;
	PUINT16(buf, 2) = 4 + len;
	memcpy(PCHAR(buf, 4), packet->param.Data, len);
	return 4 + len;
}
// send Wis Message??? 
TO_DC(0x0097)
{
	WisMessage* msg = (WisMessage*)packet->param.Data;
	PUINT16(buf, 0) = 0x0097;
	size_t res = 4 + MAX_NAME_LEN + strlen(msg->_msg) + 1;
	PUINT16(buf, 2) = res;
	memcpy(PCHAR(buf, 4), packet->param.Data, res - 4);
	return res;
}
// Send out look update to client.
TO_DC(0x01d7)
{
	RoLookUpdate* update = (RoLookUpdate*)packet->param.Data;
	PUINT16(buf, 0) = 0x01d7;
	PUINT32(buf, 2) = update->_account_id;
	PUINT8(buf, 6) = update->_type;
	PUINT16(buf, 7) = update->_id1;
	PUINT16(buf, 9) = update->_id2;

	return 11;
}

TO_DC(0x0201)
{
	size_t res = 0;
	RoFriendList* list = (RoFriendList*)packet->param.Data;
	if (list->_friends->empty())
	{
		return 0;
	}

	size_t count = list->_friends->size();
	// make the list.
	PUINT16(buf, res) = 0x0201;		res += 2;
	PUINT16(buf, res) = 32 * count + 4;		res += 2;
	for (size_t i = 0; i < count; ++i)
	{
		PUINT32(buf, res) = (*list->_friends)[i]._account_id;	res += 4;
		PUINT32(buf, res) = (*list->_friends)[i]._char_id;		res += 4;
		memcpy(PCHAR(buf, res), (*list->_friends)[i]._name.c_str(), (*list->_friends)[i]._name.size());
		res += 24; // max_name_len.
	}
	assert(res == 32 * count + 4);
	return res;
}

TO_DC(0x0206)
{
	RoFriendStatus* status = (RoFriendStatus*)packet->param.Data;
	PUINT16(buf, 0) = 0x0206;
	PUINT32(buf, 2) = status->_account_id;
	PUINT32(buf, 6) = status->_char_id;
	PUINT8(buf, 10) = status->_online ? 0 : 1;

	return 11;
}

// send equip item list.
TO_DC(0x00a4)
{
	size_t res = 0;
	RoItemList* list = (RoItemList*) packet->param.Data;
	size_t count = (*list->_items).size();
	if (count == 0)
		return 0;

	PUINT16(buf, res) = 0x00a4;		res += 2;
	PUINT16(buf, res) = count * 20 + 4;		res += 2;
	for (size_t i = 0; i < count; ++i)
	{
		PUINT16(buf, res) = i + 2; // for equippable.
		res += 2;
		// Item info.
		PUINT16(buf, res) = (*list->_items)[i]._info->_type;
		res += 2;
		PUINT8(buf, res) = (*list->_items)[i]._info->_item_type;
		res += 1;
		PUINT8(buf, res) = (*list->_items)[i]._info->_identify;
		res += 1;
		PUINT16(buf, res) = (*list->_items)[i]._equip_point;
		res += 2;
		PUINT16(buf, res) = (*list->_items)[i]._info->_equip;
		res += 2;
		PUINT8(buf, res) = (*list->_items)[i]._info->_attrs;
		res += 1;
		PUINT8(buf, res) = (*list->_items)[i]._info->_refine;
		res += 1;
		// TODO: add cards information.
		res += 8;
	}
	assert(res == count * 20 + 4);
	return res;
}
TO_DC(0x01ee)
{
	size_t res = 0;
	RoItemList* list = (RoItemList*) packet->param.Data;
	size_t count = (*list->_items).size();
	if (count == 0)
		return 0;

	PUINT16(buf, res) = 0x01ee;		res += 2;
	PUINT16(buf, res) = count * 18 + 4;

	for (size_t i = 0; i < count; ++i)
	{
		PUINT16(buf, res) = i + 2; // for equippable.
		res += 2;
		// Item info.
		PUINT16(buf, res) = (*list->_items)[i]._info->_type;
		res += 2;
		PUINT8(buf, res) = (*list->_items)[i]._info->_item_type;
		res += 1;
		PUINT8(buf, res) = (*list->_items)[i]._info->_identify;
		res += 1;
		PUINT16(buf, res) = (*list->_items)[i]._info->_amount;
		res += 2;
		PUINT16(buf, res) = (*list->_items)[i]._info->_equip == EQP_AMMO ? EQP_AMMO : 0;
		res += 2;

		// TODO: add cards. 
		res += 8;	
	}

	assert(res == count * 18 + 4);
	return res;
}
// Send walk to xy ok
TO_DC(0x0087)
{
	PUINT16(buf, 0) = 0x0087;

	RoWalkToXY_OK* ok = (RoWalkToXY_OK*)packet->param.Data;
	PUINT16(buf, 2) = ok->_tick;
	SET_POS2((uint8*)buf, 6, ok->_org._x, ok->_org._y, ok->_to._x, ok->_to._y, 8, 8);
	return 12;
}

TO_DC(0x007f)
{
	PUINT16(buf, 0) = 0x007f;
	PUINT32(buf, 2) = packet->param.Int;
	return 6;
}
TO_DC(0x0086)
{
	PUINT16(buf, 0) = 0x0086;
	RoUnitMove* move = (RoUnitMove*)packet->param.Data;
	PUINT32(buf, 2) = move->_id;
	SET_POS2((uint8*)buf, 6, move->_org._x, move->_org._y, move->_to._x, move->_to._y, 8, 8);
	PUINT32(buf, 12) = move->_tick;
	return 16;
}
}

