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

// Send one item adding
TO_DC(0x00a0)
{
	size_t res = 0;
	RoItemData* data = (RoItemData*) packet->param.Data;

	if (data->_fail)
	{
		data->_id = 0;
		data->_identify = 0;
		data->_attrs = 0;
		data->_refine = 0;
		data->_equip_point = 0;
		data->_type = 0;
	}
	PUINT16(buf, res) = 0x00a0;	res += 2;
	PUINT16(buf, res) = data->_index + 2;	res += 2;
	PUINT16(buf, res) = data->_amount;		res += 2;
	PUINT16(buf, res) = data->_id;			res += 2;
	PUINT8(buf, res) = data->_identify;		res += 1;
	PUINT8(buf, res) = data->_attrs;		res += 1;
	PUINT8(buf, res) = data->_refine;		res += 1;
	memset(PCHAR(buf, res), 0, 8);
	// TODO: for cards;
	res += 8;
	PUINT16(buf, res) = data->_equip_point;	res += 2;
	PUINT8(buf, res) = data->_type;			res += 1;
	PUINT8(buf, res) = data->_fail;			res += 1;

	return res;
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
		PUINT16(buf, res) = (*list->_items)[i]._id;
		res += 2;
		PUINT8(buf, res) = (*list->_items)[i]._type;
		res += 1;
		PUINT8(buf, res) = (*list->_items)[i]._identify;
		res += 1;
		PUINT16(buf, res) = (*list->_items)[i]._equip_point;
		res += 2;
		PUINT16(buf, res) = (*list->_items)[i]._equiped_point;
		res += 2;
		PUINT8(buf, res) = (*list->_items)[i]._attrs;
		res += 1;
		PUINT8(buf, res) = (*list->_items)[i]._refine;
		res += 1;
		memset(PCHAR(buf, res), 0, 8);
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
	PUINT16(buf, res) = count * 18 + 4; res += 2;

	for (size_t i = 0; i < count; ++i)
	{
		PUINT16(buf, res) = i + 2; //stackable items.
		res += 2;
		// Item info.
		PUINT16(buf, res) = (*list->_items)[i]._id;
		res += 2;
		PUINT8(buf, res) = (*list->_items)[i]._type;
		res += 1;
		PUINT8(buf, res) = (*list->_items)[i]._identify;
		res += 1;
		PUINT16(buf, res) = (*list->_items)[i]._amount;
		res += 2;
		PUINT16(buf, res) = (*list->_items)[i]._equiped_point == EQP_AMMO ? EQP_AMMO : 0;
		res += 2;

		// TODO: add cards. 
		memset(PCHAR(buf, res), 0, 8);
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
TO_DC(0x00aa)
{
	RoEquipItemOK* ok = (RoEquipItemOK*)packet->param.Data;
	PUINT16(buf, 0) = 0x00aa;
	PUINT16(buf, 2) = ok->_index + 2;
	PUINT16(buf, 4) = ok->_pos;
	PUINT8(buf, 6) = ok->_ok;
	return 7;
}
TO_DC(0x00ac)
{
	RoUnequipItemOK* ok = (RoUnequipItemOK*)packet->param.Data;
	PUINT16(buf, 0) = 0x00ac;
	PUINT16(buf, 2) = ok->_index + 2;
	PUINT16(buf, 4) = ok->_pos;
	PUINT8(buf, 6) = ok->_ok;
	return 7;
}

// actions and attack damage.
TO_DC(0x008a)
{
	RoActionData* data = (RoActionData*)packet->param.Data;
	size_t res = 0;

	PUINT16(buf, res) = 0x008a;			res += 2;
	PUINT32(buf, res) = data->_src_id;	res += 4;
	PUINT32(buf, res) = data->_dst_id;	res += 4;
	PUINT32(buf, res) = data->_tick_count;	res += 4;
	PUINT32(buf, res) = data->_src_delay;	res += 4;
	PUINT32(buf, res) = data->_dst_delay;	res += 4;
	PUINT16(buf, res) = data->_damage; res += 2; // TODO: for div
	PUINT16(buf, res) = data->_div;	res += 2;
	PUINT8(buf, res) = data->_type;	res += 1;
	PUINT16(buf, res) = data->_damage2; res + 2;

	return res; // 29
}
// Spawning new player
TO_DC(0x007c) // clif.c:747
{
	RoCharInfoBase* info = (RoCharInfoBase*) packet->param.Data;
	size_t res = 0;

	PUINT16(buf, res) = 0x007c;		res += 2;

	PUINT8(buf, res) = 0;			res += 1;
	PUINT32(buf, res) = info->_id;	res += 4;
	PUINT16(buf, res) = info->_status._speed;	res += 2;
	PUINT16(buf, res) = info->_status._body_state;	res += 2; //11
	PUINT16(buf, res) = info->_status._health_state; res += 2;
	PUINT16(buf, res) = info->_status._option;	res += 2;
	PUINT16(buf, res) = info->_show._hair_style;	res += 2;
	PUINT16(buf, res) = info->_show._weapon; 	res += 2;
	PUINT16(buf, res) = info->_show._head_bottom;	res += 2; //21
	PUINT16(buf, res) = info->_class;	res += 2;
	PUINT16(buf, res) = info->_show._shield;	res += 2;
	PUINT16(buf, res) = info->_show._head_top;		res += 2;
	PUINT16(buf, res) = info->_show._head_middle;		res += 2;
	// TODO:
	PUINT16(buf, res) = info->_show._hair_color;		res += 2; //31
	PUINT16(buf, res) = info->_show._clothes_color;	res += 2;
	PUINT16(buf, res) = info->_status._head_dir;		res += 2;
	PUINT8(buf, res) = 0; // karma
	res += 1;
	PUINT8(buf, res) = info->_show._sex;	res += 1;
	SET_POS((uint8*)buf, res, info->_last_pos._x, info->_last_pos._y, info->_status._head_dir);	res += 3;
	PUINT8(buf, res) = 0;		res += 1; // 41
	PUINT8(buf, res) = 0;		res += 1;
	// totally 42;
	assert(res == 42);
	return res;
}

TO_DC(0x0078) // Player standing
{
	RoCharInfoBase* info = (RoCharInfoBase*)packet->param.Data;
	size_t res = 0;

	PUINT16(buf, res) = 0x0078;		res += 2;

	//
	PUINT8(buf, res) = 0;			res += 1;
	PUINT32(buf, res) = info->_id;	res += 4;
	PUINT16(buf, res) = info->_status._speed;	res += 2;
	PUINT16(buf, res) = info->_status._body_state;	res += 2;  // 11
	PUINT16(buf, res) = info->_status._health_state;	res += 2;
	PUINT16(buf, res) = info->_status._option;		res += 2;
	PUINT16(buf, res) = info->_class;		res += 2;
	PUINT16(buf, res) = info->_show._hair_style;	res += 2;
	PUINT16(buf, res) = info->_show._weapon;		res += 2; //21
	PUINT16(buf, res) = info->_show._shield;		res += 2;
	PUINT16(buf, res) = info->_show._head_bottom;	res += 2;
	PUINT16(buf, res) = info->_show._head_top;		res += 2;
	PUINT16(buf, res) = info->_show._head_middle;		res += 2;
	// TODO:
	PUINT16(buf, res) = info->_show._hair_color;	res += 2; // 31
	PUINT16(buf, res) = info->_show._clothes_color;	res += 2;
	PUINT16(buf, res) = info->_status._head_dir;	res += 2;
	PUINT32(buf, res) = 0; res += 4;// TODO:
	PUINT16(buf, res) = 0;	res += 2; //TODO: 					//41
	PUINT16(buf, res) = info->_prop._manner;	res += 2;
	PUINT16(buf, res) = info->_status._skill_state;	res += 2;
	PUINT8(buf, res) = info->_prop._karma;	res += 1;
	PUINT8(buf, res) = info->_show._sex;	res += 1;
	SET_POS((uint8*)buf, res, info->_last_pos._x, info->_last_pos._y,  info->_status._head_dir);	res += 3;
	PUINT8(buf, res) = 5;	res += 1; 						// 51
	PUINT8(buf, res) = 5;	res += 1;
	PUINT8(buf, res) = info->_status._state.dead_sit;	res += 1;
	PUINT16(buf, res) = info->_exp._base_lvl;			res += 2;

	assert(res == 55);
	return res;
}

TO_DC(0x022b) // spawning new mob.  // clif.c:770
{
	RoCharInfoBase* info = (RoCharInfoBase*) packet->param.Data;
	size_t res = 0;

	PUINT16(buf, res) = 0x022b;		res += 2;
	PUINT32(buf, res) = info->_id;	res += 4;
	PUINT16(buf, res) = info->_status._speed;	res += 2;
	PUINT16(buf, res) = info->_status._body_state;	res += 2; //10
	PUINT16(buf, res) = info->_status._health_state;	res += 2; 
	PUINT32(buf, res) = info->_status._option;		res += 4;
	PUINT16(buf, res) = info->_class;		res += 2;
	PUINT16(buf, res) = info->_show._hair_style;	res += 2; // 20
	PUINT16(buf, res) = info->_show._weapon;		res += 2;
	PUINT16(buf, res) = info->_show._shield;		res += 2;
	PUINT16(buf, res) = info->_show._head_bottom;	res += 2;
	PUINT16(buf, res) = info->_show._head_top;		res += 2;
	PUINT16(buf, res) = info->_show._head_middle;		res += 2; // 30
	// TODO:  There is difference about BL_NPC && _class == FLAG_CLASS.
	PUINT16(buf, res) = info->_show._hair_color;		res += 2;
	PUINT16(buf, res) = info->_show._clothes_color;	res += 2;
	PUINT16(buf, res) = info->_status._head_dir;		res += 2;

	// TODO: Below are clif_visual_guild_id() and clif_visual_emblem_id
	PUINT32(buf, res) = 0;		res += 4;					// 40
	PUINT16(buf, res) = 0;		res += 2; 
	PUINT16(buf, res) = info->_prop._manner;	res += 2;
	PUINT32(buf, res) = info->_status._skill_state;	res += 2; // why?
	PUINT16(buf, res) = info->_status._skill_state;	res += 2;
	PUINT8(buf, res) = info->_prop._karma;	res += 1;
	PUINT8(buf, res) = info->_show._sex;		res += 1; // 50
	SET_POS((uint8*)buf, res, info->_last_pos._x, info->_last_pos._y, info->_status._head_dir);		res += 3;
	PUINT8(buf, res) = 5; res += 1; // TODO:
	PUINT8(buf, res) = 5; res += 1; // TODO:
	PUINT16(buf, res) = info->_exp._base_lvl;	res += 2;
	assert(res == 57);

	return res;
}
TO_DC(0x022a) // mob standing
{
	RoCharInfoBase* info = (RoCharInfoBase*) packet->param.Data;
	size_t res = 0;

	PUINT16(buf, res) = 0x022a;		res += 2;
	PUINT32(buf, res) = info->_id;	res += 4;
	PUINT16(buf, res) = info->_status._speed;	res += 2;
	PUINT16(buf, res) = info->_status._body_state;		res += 2; //10
	PUINT16(buf, res) = info->_status._health_state;	res += 2;
	PUINT32(buf, res) = info->_status._option;			res += 4; 
	PUINT16(buf, res) = info->_class;					res += 2;
	PUINT16(buf, res) = info->_show._hair_style;		res += 2; // 20
	PUINT16(buf, res) = info->_show._weapon;			res += 2;
	PUINT16(buf, res) = info->_show._shield;			res += 2;
	PUINT16(buf, res) = info->_show._head_bottom;		res += 2;
	PUINT16(buf, res) = info->_show._head_top;			res += 2;
	PUINT16(buf, res) = info->_show._head_middle;			res += 2; //30
	// TODO: The BL_NPC && FLAG_CLASS
	PUINT16(buf, res) = info->_show._hair_color;		res += 2;
	PUINT16(buf, res) = info->_show._clothes_color;		res += 2;
	PUINT16(buf, res) = info->_status._head_dir;			res += 2;
	PUINT32(buf, res) = 0;	res += 4;	//TODO:clif_visual_guild_id;  // 40
	PUINT16(buf, res) = 0;	res += 2;	// TODO:clif_visual_emblem_id 
	PUINT16(buf, res) = info->_prop._manner;	res += 2;
	PUINT32(buf, res) = info->_status._skill_state;		res += 4;
	PUINT8(buf, res) = info->_prop._karma;				res += 1;
	PUINT8(buf, res) = info->_show._sex;				res += 1; //50
	SET_POS((uint8*)buf, res, info->_last_pos._x, info->_last_pos._y, info->_status._head_dir);		res += 3;
	PUINT8(buf, res) = 5;		res += 1;
	PUINT8(buf, res) = 5;		res += 1;
	PUINT8(buf, res) = info->_status._state.dead_sit;		res += 1;
	PUINT16(buf, res) = info->_exp._base_lvl;			res += 2;

	assert(res == 58);
	return res;
}
}

