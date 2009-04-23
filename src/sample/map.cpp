#include "map.h"
#include "share.h"
#include "ro_defs.h"
#include <cstring>
#include <cstdio>

namespace ro
{
//size_t from_map_connect(Packet* dest, const char* src, size_t size)
DECLARE(map_connect)
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
	if (size < len)
		return 0;

	ConnectToMap* ctm = new ConnectToMap();
	ctm->_ver = op;
	ctm->_account_id = PUINT32(src, index[0]);
	printf("===================account_id : %d===================", ctm->_account_id);
	ctm->_char_id = PUINT32(src, index[1]);
	printf("====================Char_id : %d=====================", ctm->_char_id);
	ctm->_login_id1 = PUINT32(src, index[2]);
	ctm->_client_tick = PUINT32(src, index[3]);
	ctm->_sex = PUINT8(src, index[4]);

	dest->op = EC_CONNECT_TO_MAP;
	dest->len = sizeof(Packet) + sizeof(ConnectToMap);
	dest->param.Data = (char*)ctm;

	return len;
}
/*
size_t to_map_connect(char* buf, const Packet* packet)
{
	if (packet->param.Char)
	{
		return send_6a_to_disconnect(buf, (uint8)packet->param.Char);
	}
	else
	{
		PUINT16(buf, 0) = 0x0283;
		//PUINT(src, 2) = packet
	}
}*/
DECLARE(load_end_ack)
{
	dest->op = EC_LOAD_END_ACK;
	dest->len = sizeof(Packet);
	dest->param.Long = 0;
	return 2;
}
DECLARE(tick_send)
{
	if (size < 8)
		return 0;

	dest->op = EC_TICK_COUNT;
	dest->len = sizeof(Packet);
	dest->param.Int = PUINT32(src, 4); // only support version after 2007-02-12.
	return 4 + 4;
}
DECLARE(walk_to_xy)
{
	if (size < 8)
		return 0;

	dest->op = EC_WALK_TO;
	dest->len = sizeof(Packet);
	RoWalkToXY xy;
	int pos = 5;
	xy._point._x = (PUINT8(src, pos) * 4) + (PUINT8(src, pos + 1) >> 6);
	xy._point._y = ((PUINT8(src, pos + 1) & 0x3f) << 4) + (PUINT8(src, pos + 2) >> 4);
	dest->param.Int = xy._int;
	return 8;
}
DECLARE(quit_game)
{
	dest->op = EC_QUIT_GAME;
	dest->len = sizeof(Packet);
	dest->param.Int = PUINT16(src, 2);
	return 4;
}

DECLARE(get_char_name_request)
{
	dest->op = EC_REQUEST_CHAR_NAME;
	dest->len = sizeof(Packet);
	dest->param.Int = PUINT32(src, 7);
	return 11;
}

DECLARE(global_message)
{
	size_t textlen = PUINT16(src, 2) - 4;
	if (size < textlen)
		return 0;

	dest->op = EC_MESSAGE;
	dest->len = sizeof(Packet) + textlen;
	dest->param.Data = new char[textlen];
	memcpy((char*)dest->param.Data, PCHAR(src, 4), textlen);

	return textlen + 4;
}

DECLARE(map_move)
{
	return 0;
}

DECLARE(change_dir)
{
	RoChangeDirUnion c;
	c._dir._head_dir = *(PCHAR(src, 7));
	c._dir._dir = *(PCHAR(src, 10));
	dest->op = EC_CHANGE_DIR;
	dest->len = sizeof(Packet);
	dest->param.Long = c._long;
	return 11;
}

DECLARE(emotion)
{
	return 0;
}

DECLARE(how_many_connections)
{
	return 0;
}

DECLARE(action_request)
{
	return 0;
}

DECLARE(restart)
{
	return 0;
}

DECLARE(wis)
{
	return 0;
}

DECLARE(gm_message)
{
	return 0;
}

DECLARE(take_item)
{
	return 0;
}

DECLARE(drop_item)
{
	return 0;
}

DECLARE(use_item)
{
	return 0;
}

DECLARE(equip_item)
{
	RoEquipItemUnion item;
	item._item._index = PUINT16(src, 2) - 2;
	item._item._pos = PUINT16(src, 4);
	dest->op = EC_EQUIP_ITEM;
	dest->len = sizeof(Packet);
	dest->param.Long = item._long;
	return 6;
}

DECLARE(unequip_item)
{
	dest->op = EC_UNEQUIP_ITEM;
	dest->len = sizeof(Packet);
	dest->param.Short = PINT16(src, 2) - 2;
	return 4;
}

DECLARE(npc_clicked)
{
	dest->op = EC_CLICK_NPC;
	dest->len = sizeof(Packet);
	dest->param.Int = PUINT32(src, 2);

	// TODO: what is the last byte?
	return 7;
}

DECLARE(npc_buy_sell_selected)
{
	dest->op = EC_NPC_BUY_SELL_SELECT;
	dest->len = sizeof(Packet) + sizeof(RoNpcBuySellSelect);
	RoNpcBuySellSelect* s = new RoNpcBuySellSelect();
	s->_npc_id = PUINT32(src, 2);
	s->_flag = PUINT8(src, 6);
	dest->param.Data = (char*)s;
	return 7;
}

DECLARE(npc_buy_list_send)
{
	return 0;
}

DECLARE(npc_sell_list_send)
{
	return 0;
}

DECLARE(create_chat_room)
{
	return 0;
}

DECLARE(chat_add_member)
{
	return 0;
}

DECLARE(chat_room_status_change)
{
	return 0;
}

DECLARE(change_chat_owner)
{
	return 0;
}

DECLARE(kick_from_chat)
{
	return 0;
}

DECLARE(chat_leave)
{
	return 0;
}

DECLARE(trade_request)
{
	return 0;
}

DECLARE(trade_ack)
{
	return 0;
}

DECLARE(trade_add_item)
{
	return 0;
}

DECLARE(trade_ok)
{
	return 0;
}

DECLARE(trade_cancel)
{
	return 0;
}

DECLARE(trade_commit)
{
	return 0;
}

DECLARE(stop_attack)
{
	return 0;
}

DECLARE(put_item_to_cart)
{
	return 0;
}

DECLARE(get_item_from_cart)
{
	return 0;
}

DECLARE(remove_option)
{
	return 0;
}

DECLARE(change_cart)
{
	return 0;
}

DECLARE(status_up)
{
	return 0;
}

DECLARE(skill_up)
{
	return 0;
}

DECLARE(use_skill_to_id)
{
	return 0;
}

DECLARE(use_skill_to_pos)
{
	return 0;
}

DECLARE(use_skill_to_pos_info)
{
	return 0;
}

DECLARE(use_skill_map)
{
	return 0;
}

DECLARE(request_memo)
{
	return 0;
}

DECLARE(produce_mix)
{
	return 0;
}

DECLARE(npc_select_menu)
{
	return 0;
}

DECLARE(npc_next_clicked)
{
	return 0;
}

DECLARE(npc_amount_input)
{
	return 0;
}

DECLARE(npc_string_input)
{
	return 0;
}

DECLARE(npc_close_clicked)
{
	return 0;
}

DECLARE(item_identify)
{
	return 0;
}

DECLARE(select_arrow)
{
	return 0;
}

DECLARE(auto_spell)
{
	return 0;
}

DECLARE(use_card)
{
	return 0;
}

DECLARE(insert_card)
{
	return 0;
}

DECLARE(repair_item)
{
	return 0;
}

DECLARE(weapon_refine)
{
	return 0;
}

DECLARE(solve_char_name)
{
	return 0;
}

DECLARE(reset_char)
{
	return 0;
}

DECLARE(lgm_message)
{
	return 0;
}

DECLARE(move_to_kafra)
{
	return 0;
}

DECLARE(move_from_kafra)
{
	return 0;
}

DECLARE(move_to_kafra_from_cart)
{
	return 0;
}

DECLARE(move_from_kafra_to_cart)
{
	return 0;
}

DECLARE(close_kafra)
{
	return 0;
}

DECLARE(create_party)
{
	return 0;
}

DECLARE(create_party2)
{
	return 0;
}

DECLARE(party_invite)
{
	return 0;
}

DECLARE(party_invite2)
{
	return 0;
}

DECLARE(reply_party_invite)
{
	return 0;
}

DECLARE(reply_party_invite2)
{
	return 0;
}

DECLARE(leave_party)
{
	return 0;
}

DECLARE(remove_party_member)
{
	return 0;
}

DECLARE(party_change_option)
{
	return 0;
}

DECLARE(party_message)
{
	return 0;
}

DECLARE(close_vending)
{
	return 0;
}

DECLARE(vending_list_req)
{
	return 0;
}

DECLARE(purchase_req)
{
	return 0;
}

DECLARE(open_vending)
{
	return 0;
}

DECLARE(create_guild)
{
	return 0;
}

DECLARE(guild_check_master)
{
	dest->op = EC_GUILD_CHECK_MASTER;
	dest->len = sizeof(Packet);
	dest->param.Long = 0;
	return 2;
}

DECLARE(guild_request_info)
{
	dest->op = EC_GUILD_REQUEST_INFO;
	dest->len = sizeof(Packet);
	dest->param.Int = PUINT32(src, 2);
	return 6;
}

DECLARE(guild_change_position_info)
{
	return 0;
}

DECLARE(guild_change_member_position)
{
	return 0;
}

DECLARE(guild_request_emblem)
{
	return 0;
}

DECLARE(guild_change_emblem)
{
	return 0;
}

DECLARE(guild_change_notice)
{
	return 0;
}

DECLARE(guild_invite)
{
	return 0;
}

DECLARE(guild_reply_invite)
{
	return 0;
}

DECLARE(guild_leave)
{
	return 0;
}

DECLARE(guild_expulsion)
{
	return 0;
}

DECLARE(guild_message)
{
	return 0;
}

DECLARE(guild_request_all_iance)
{
	return 0;
}

DECLARE(guild_reply_all_iance)
{
	return 0;
}

DECLARE(guild_del_all_iance)
{
	return 0;
}

DECLARE(guild_op_position)
{
	return 0;
}

DECLARE(guild_break)
{
	return 0;
}

DECLARE(pet_menu)
{
	return 0;
}

DECLARE(catch_pet)
{
	return 0;
}

DECLARE(select_egg)
{
	return 0;
}

DECLARE(send_emotion)
{
	return 0;
}

DECLARE(change_pet_name)
{
	return 0;
}

DECLARE(gm_kick)
{
	return 0;
}

DECLARE(gm_hide)
{
	return 0;
}

DECLARE(gm_req_nochat)
{
	return 0;
}

DECLARE(gm_req_acc_name)
{
	return 0;
}

DECLARE(gm_kill_all)
{
	return 0;
}

DECLARE(gm_recall)
{
	return 0;
}

DECLARE(gm_summon)
{
	return 0;
}

DECLARE(gm_item_monster)
{
	return 0;
}

DECLARE(gm_remove)
{
	return 0;
}

DECLARE(gm_shift)
{
	return 0;
}

DECLARE(gm_change_map_type)
{
	return 0;
}

DECLARE(gm_rc)
{
	return 0;
}

DECLARE(sn_doridori)
{
	return 0;
}

DECLARE(sn_explosion_spirits)
{
	return 0;
}

DECLARE(pm_ignore)
{
	return 0;
}

DECLARE(pm_ignore_list)
{
	return 0;
}

DECLARE(pm_ignore_all)
{
	return 0;
}

DECLARE(friends_list_add)
{
	return 0;
}

DECLARE(friends_list_remove)
{
	return 0;
}

DECLARE(friends_list_reply)
{
	return 0;
}

DECLARE(black_smith)
{
	return 0;
}

DECLARE(alchemist)
{
	return 0;
}

DECLARE(taekwon)
{
	return 0;
}

DECLARE(ranking_pk)
{
	return 0;
}

DECLARE(feel_save_ok)
{
	return 0;
}

DECLARE(debug)
{
	return 0;
}

DECLARE(change_hom_unculus_name)
{
	return 0;
}

DECLARE(hom_move_to_master)
{
	return 0;
}

DECLARE(hom_move_to)
{
	return 0;
}

DECLARE(hom_attack)
{
	return 0;
}

DECLARE(hom_menu)
{
	return 0;
}

DECLARE(storage_password)
{
	return 0;
}

DECLARE(hotkey)
{
	return 0;
}

DECLARE(auto_revive)
{
	return 0;
}

DECLARE(check)
{
	return 0;
}

DECLARE(adopt_request)
{
	return 0;
}

DECLARE(adopt_reply)
{
	return 0;
}

DECLARE(mail_refresh)
{
	return 0;
}

DECLARE(mail_read)
{
	return 0;
}

DECLARE(mail_get_attach)
{
	return 0;
}

DECLARE(mail_delete)
{
	return 0;
}

DECLARE(mail_return)
{
	return 0;
}

DECLARE(mail_set_attach)
{
	return 0;
}

DECLARE(mail_win_open)
{
	return 0;
}

DECLARE(mail_send)
{
	return 0;
}

DECLARE(auction_search)
{
	return 0;
}

DECLARE(auction_buy_sell)
{
	return 0;
}

DECLARE(auction_set_item)
{
	return 0;
}

DECLARE(auction_cancel_reg)
{
	return 0;
}

DECLARE(auction_register)
{
	return 0;
}

DECLARE(auction_cancel)
{
	return 0;
}

DECLARE(auction_close)
{
	return 0;
}

DECLARE(auction_bid)
{
	return 0;
}

DECLARE(cash_shop_buy)
{
	return 0;
}

DECLARE(view_player_equip)
{
	return 0;
}

DECLARE(equip_tick_box)
{
	return 0;
}

DECLARE(battle_chat)
{
	return 0;
}

DECLARE(mer_menu)
{
	return 0;
}



}
