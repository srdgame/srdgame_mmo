#include "fromdispatch.h"
#include "login.h"
#include "char.h"
#include "map.h"
#include "packetdb.h"
#include "conversion_cast.h"

using namespace ro;
using namespace std;

#define FROM_DB_FILE_NAME "./data/db/from_packet_db.txt"

static struct {
	FromFunctionPtr ptr;
	const char *name;
} from_parse_func[]={
	{from_map_connect, "wanttoconnection"},
	{from_load_end_ack, "loadendack"},
	{from_tick_send, "ticksend"},
	{from_walk_to_xy, "walktoxy"},
	{from_quit_game, "quitgame"},
	{from_get_char_name_request, "getcharnamerequest"},
	{from_global_message, "globalmessage"},
	{from_map_move, "mapmove"},
	{from_change_dir, "changedir"},
	{from_emotion, "emotion"},
	{from_how_many_connections, "howmanyconnections"},
	{from_action_request, "actionrequest"},
	{from_restart, "restart"},
	{from_wis, "wis"},
	{from_gm_message, "gmmessage"},
	{from_take_item, "takeitem"},
	{from_drop_item, "dropitem"},
	{from_use_item, "useitem"},
	{from_equip_item, "equipitem"},
	{from_unequip_item, "unequipitem"},
	{from_npc_clicked, "npcclicked"},
	{from_npc_buy_sell_selected, "npcbuysellselected"},
	{from_npc_buy_list_send, "npcbuylistsend"},
	{from_npc_sell_list_send, "npcselllistsend"},
	{from_create_chat_room, "createchatroom"},
	{from_chat_add_member, "chataddmember"},
	{from_chat_room_status_change, "chatroomstatuschange"},
	{from_change_chat_owner, "changechatowner"},
	{from_kick_from_chat, "kickfromchat"},
	{from_chat_leave, "chatleave"},
	{from_trade_request, "traderequest"},
	{from_trade_ack, "tradeack"},
	{from_trade_add_item, "tradeadditem"},
	{from_trade_ok, "tradeok"},
	{from_trade_cancel, "tradecancel"},
	{from_trade_commit, "tradecommit"},
	{from_stop_attack, "stopattack"},
	{from_put_item_to_cart, "putitemtocart"},
	{from_get_item_from_cart, "getitemfromcart"},
	{from_remove_option, "removeoption"},
	{from_change_cart, "changecart"},
	{from_status_up, "statusup"},
	{from_skill_up, "skillup"},
	{from_use_skill_to_id, "useskilltoid"},
	{from_use_skill_to_pos, "useskilltopos"},
	{from_use_skill_to_pos_info, "useskilltoposinfo"},
	{from_use_skill_map, "useskillmap"},
	{from_request_memo, "requestmemo"},
	{from_produce_mix, "producemix"},
	{from_npc_select_menu, "npcselectmenu"},
	{from_npc_next_clicked, "npcnextclicked"},
	{from_npc_amount_input, "npcamountinput"},
	{from_npc_string_input, "npcstringinput"},
	{from_npc_close_clicked, "npccloseclicked"},
	{from_item_identify, "itemidentify"},
	{from_select_arrow, "selectarrow"},
	{from_auto_spell, "autospell"},
	{from_use_card, "usecard"},
	{from_insert_card, "insertcard"},
	{from_repair_item, "repairitem"},
	{from_weapon_refine, "weaponrefine"},
	{from_solve_char_name, "solvecharname"},
	{from_reset_char, "resetchar"},
	{from_lgm_message, "lgmmessage"},
	{from_move_to_kafra, "movetokafra"},
	{from_move_from_kafra, "movefromkafra"},
	{from_move_to_kafra_from_cart, "movetokafrafromcart"},
	{from_move_from_kafra_to_cart, "movefromkafratocart"},
	{from_close_kafra, "closekafra"},
	{from_create_party, "createparty"},
	{from_create_party2, "createparty2"},
	{from_party_invite, "partyinvite"},
	{from_party_invite2, "partyinvite2"},
	{from_reply_party_invite, "replypartyinvite"},
	{from_reply_party_invite2, "replypartyinvite2"},
	{from_leave_party, "leaveparty"},
	{from_remove_party_member, "removepartymember"},
	{from_party_change_option, "partychangeoption"},
	{from_party_message, "partymessage"},
	{from_close_vending, "closevending"},
	{from_vending_list_req, "vendinglistreq"},
	{from_purchase_req, "purchasereq"},
	{from_open_vending, "openvending"},
	{from_create_guild, "createguild"},
	{from_guild_check_master, "guildcheckmaster"},
	{from_guild_request_info, "guildrequestinfo"},
	{from_guild_change_position_info, "guildchangepositioninfo"},
	{from_guild_change_member_position, "guildchangememberposition"},
	{from_guild_request_emblem, "guildrequestemblem"},
	{from_guild_change_emblem, "guildchangeemblem"},
	{from_guild_change_notice, "guildchangenotice"},
	{from_guild_invite, "guildinvite"},
	{from_guild_reply_invite, "guildreplyinvite"},
	{from_guild_leave, "guildleave"},
	{from_guild_expulsion, "guildexpulsion"},
	{from_guild_message, "guildmessage"},
	{from_guild_request_all_iance, "guildrequestalliance"},
	{from_guild_reply_all_iance, "guildreplyalliance"},
	{from_guild_del_all_iance, "guilddelalliance"},
	{from_guild_op_position, "guildopposition"},
	{from_guild_break, "guildbreak"},
	{from_pet_menu, "petmenu"},
	{from_catch_pet, "catchpet"},
	{from_select_egg, "selectegg"},
	{from_send_emotion, "sendemotion"},
	{from_change_pet_name, "changepetname"},
	{from_gm_kick, "gmkick"},
	{from_gm_hide, "gmhide"},
	{from_gm_req_nochat, "gmreqnochat"},
	{from_gm_req_acc_name, "gmreqaccname"},
	{from_gm_kill_all, "killall"},
	{from_gm_recall, "recall"},
	{from_gm_summon, "summon"},
	{from_gm_item_monster, "itemmonster"},
	{from_gm_remove, "remove"},
	{from_gm_shift, "shift"},
	{from_gm_change_map_type, "changemaptype"},
	{from_gm_rc, "rc"},
	{from_sn_doridori, "sndoridori"},
	{from_sn_explosion_spirits, "snexplosionspirits"},
	{from_pm_ignore, "wisexin"},
	{from_pm_ignore_list, "wisexlist"},
	{from_pm_ignore_all, "wisall"},
	{from_friends_list_add, "friendslistadd"},
	{from_friends_list_remove, "friendslistremove"},
	{from_friends_list_reply, "friendslistreply"},
	{from_black_smith, "blacksmith"},
	{from_alchemist, "alchemist"},
	{from_taekwon, "taekwon"},
	{from_ranking_pk, "rankingpk"},
	{from_feel_save_ok, "feelsaveok"},
	{from_debug, "debug"},
	{from_change_hom_unculus_name, "changehomunculusname"},
	{from_hom_move_to_master, "hommovetomaster"},
	{from_hom_move_to, "hommoveto"},
	{from_hom_attack, "homattack"},
	{from_hom_menu, "hommenu"},
	{from_storage_password, "storagepassword"},
	{from_hotkey, "hotkey"},
	{from_auto_revive, "autorevive"},
	{from_check, "check"},
	{from_adopt_request, "adoptrequest"},
	{from_adopt_reply, "adoptreply"},
	// MAIL SYSTEM
	{from_mail_refresh, "mailrefresh"},
	{from_mail_read, "mailread"},
	{from_mail_get_attach, "mailgetattach"},
	{from_mail_delete, "maildelete"},
	{from_mail_return, "mailreturn"},
	{from_mail_set_attach, "mailsetattach"},
	{from_mail_win_open, "mailwinopen"},
	{from_mail_send, "mailsend"},
	// AUcTION SYSTEM
	{from_auction_search, "auctionsearch"},
	{from_auction_buy_sell, "auctionbuysell"},
	{from_auction_set_item, "auctionsetitem"},
	{from_auction_cancel_reg, "auctioncancelreg"},
	{from_auction_register, "auctionregister"},
	{from_auction_cancel, "auctioncancel"},
	{from_auction_close, "auctionclose"},
	{from_auction_bid, "auctionbid"},
	{from_cash_shop_buy, "cashshopbuy"},
	{from_view_player_equip, "viewplayerequip"},
	{from_equip_tick_box, "equiptickbox"},
	{from_battle_chat, "battlechat"},
	{from_mer_menu, "mermenu"},
	{NULL, NULL},
};

void FromDispatch::init()
{
	::memset(_functions, 0, sizeof(FromFunctionPtr) * MAX_FROM_INDEX);
	::memset(_packet_size, 0, sizeof(int) * MAX_FROM_INDEX);
	size_t i = 0;
	while (from_parse_func[i].ptr && from_parse_func[i].name)
	{
		_string_to_function.insert(pair<std::string, FromFunctionPtr>(string(from_parse_func[i].name), from_parse_func[i].ptr));
		++i;
	}
	//printf("_string_to_function.size() is %d\n", _string_to_function.size());
	PacketDB db(FROM_DB_FILE_NAME);
	//printf("Start to load the packet db\n");
	do
	{
		std::vector<DBField>& fields = db.fetch();
		if (fields.size() > 2)
		{
			int index = hex2int(fields[0]._val);
			//printf("Process Entry : index : %d s : %s function: %s \n", index, fields[0]._val.c_str(), fields[2]._val.c_str());
			if (index <= MAX_FROM_INDEX)
			{
				_functions[index] = _string_to_function[fields[2]._val];
				if (_functions[index])
				{
					if (index == 243)
						printf("Message entry has registered! Index(Hex): %s, \t Function: %s \t paccket size : % d\n", fields[0]._val.c_str(), fields[2]._val.c_str(), conversion_cast<int>(fields[1]._val));
				}
				else
				{
					printf("Message entry has no process function! Index(Hex): %s, \t Function: %s \t paccket size : % d\n", fields[0]._val.c_str(), fields[2]._val.c_str(), conversion_cast<int>(fields[1]._val) );
				}
			}
			else
			{
				// TODO:
			}
		}
		if (fields.size() >= 2)
		{
			int index = hex2int(fields[0]._val);
			_packet_size[index] = conversion_cast<int>(fields[1]._val);
		}
	}while(db.next());
}
