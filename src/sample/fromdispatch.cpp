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
	{from_map_connect_, "wanttoconnection"},
	{from_load_end_ack_, "loadendack"},
	{from_tick_send_, "ticksend"},
	{from_walk_to_xy_, "walktoxy"},
	{from_quit_game_, "quitgame"},
	{from_get_char_name_request_, "getcharnamerequest"},
	{from_global_message_, "globalmessage"},
	{from_map_move_, "mapmove"},
	{from_change_dir_, "changedir"},
	{from_emotion_, "emotion"},
	{from_how_many_connections_, "howmanyconnections"},
	{from_action_request_, "actionrequest"},
	{from_restart_, "restart"},
	{from_wis_, "wis"},
	{from_gm_message_, "gmmessage"},
	{from_take_item_, "takeitem"},
	{from_drop_item_, "dropitem"},
	{from_use_item_, "useitem"},
	{from_equip_item_, "equipitem"},
	{from_unequip_item_, "unequipitem"},
	{from_npc_clicked_, "npcclicked"},
	{from_npc_buy_sell_selected_, "npcbuysellselected"},
	{from_npc_buy_list_send_, "npcbuylistsend"},
	{from_npc_sell_list_send_, "npcselllistsend"},
	{from_create_chat_room_, "createchatroom"},
	{from_chat_add_member_, "chataddmember"},
	{from_chat_room_status_change_, "chatroomstatuschange"},
	{from_change_chat_owner_, "changechatowner"},
	{from_kick_from_chat_, "kickfromchat"},
	{from_chat_leave_, "chatleave"},
	{from_trade_request_, "traderequest"},
	{from_trade_ack_, "tradeack"},
	{from_trade_add_item_, "tradeadditem"},
	{from_trade_ok_, "tradeok"},
	{from_trade_cancel_, "tradecancel"},
	{from_trade_commit_, "tradecommit"},
	{from_stop_attack_, "stopattack"},
	{from_put_item_to_cart_, "putitemtocart"},
	{from_get_item_from_cart_, "getitemfromcart"},
	{from_remove_option_, "removeoption"},
	{from_change_cart_, "changecart"},
	{from_status_up_, "statusup"},
	{from_skill_up_, "skillup"},
	{from_use_skill_to_id_, "useskilltoid"},
	{from_use_skill_to_pos_, "useskilltopos"},
	{from_use_skill_to_pos_info_, "useskilltoposinfo"},
	{from_use_skill_map_, "useskillmap"},
	{from_request_memo_, "requestmemo"},
	{from_produce_mix_, "producemix"},
	{from_npc_select_menu_, "npcselectmenu"},
	{from_npc_next_clicked_, "npcnextclicked"},
	{from_npc_amount_input_, "npcamountinput"},
	{from_npc_string_input_, "npcstringinput"},
	{from_npc_close_clicked_, "npccloseclicked"},
	{from_item_identify_, "itemidentify"},
	{from_select_arrow_, "selectarrow"},
	{from_auto_spell_, "autospell"},
	{from_use_card_, "usecard"},
	{from_insert_card_, "insertcard"},
	{from_repair_item_, "repairitem"},
	{from_weapon_refine_, "weaponrefine"},
	{from_solve_char_name_, "solvecharname"},
	{from_reset_char_, "resetchar"},
	{from_lgm_message_, "lgmmessage"},
	{from_move_to_kafra_, "movetokafra"},
	{from_move_from_kafra_, "movefromkafra"},
	{from_move_to_kafra_from_cart_, "movetokafrafromcart"},
	{from_move_from_kafra_to_cart_, "movefromkafratocart"},
	{from_close_kafra_, "closekafra"},
	{from_create_party_, "createparty"},
	{from_create_party2_, "createparty2"},
	{from_party_invite_, "partyinvite"},
	{from_party_invite2_, "partyinvite2"},
	{from_reply_party_invite_, "replypartyinvite"},
	{from_reply_party_invite2_, "replypartyinvite2"},
	{from_leave_party_, "leaveparty"},
	{from_remove_party_member_, "removepartymember"},
	{from_party_change_option_, "partychangeoption"},
	{from_party_message_, "partymessage"},
	{from_close_vending_, "closevending"},
	{from_vending_list_req_, "vendinglistreq"},
	{from_purchase_req_, "purchasereq"},
	{from_open_vending_, "openvending"},
	{from_create_guild_, "createguild"},
	{from_guild_check_master_, "guildcheckmaster"},
	{from_guild_request_info_, "guildrequestinfo"},
	{from_guild_change_position_info_, "guildchangepositioninfo"},
	{from_guild_change_member_position_, "guildchangememberposition"},
	{from_guild_request_emblem_, "guildrequestemblem"},
	{from_guild_change_emblem_, "guildchangeemblem"},
	{from_guild_change_notice_, "guildchangenotice"},
	{from_guild_invite_, "guildinvite"},
	{from_guild_reply_invite_, "guildreplyinvite"},
	{from_guild_leave_, "guildleave"},
	{from_guild_expulsion_, "guildexpulsion"},
	{from_guild_message_, "guildmessage"},
	{from_guild_request_all_iance_, "guildrequestalliance"},
	{from_guild_reply_all_iance_, "guildreplyalliance"},
	{from_guild_del_all_iance_, "guilddelalliance"},
	{from_guild_op_position_, "guildopposition"},
	{from_guild_break_, "guildbreak"},
	{from_pet_menu_, "petmenu"},
	{from_catch_pet_, "catchpet"},
	{from_select_egg_, "selectegg"},
	{from_send_emotion_, "sendemotion"},
	{from_change_pet_name_, "changepetname"},
	{from_gm_kick_, "gmkick"},
	{from_gm_hide_, "gmhide"},
	{from_gm_req_nochat_, "gmreqnochat"},
	{from_gm_req_acc_name_, "gmreqaccname"},
	{from_gm_kill_all_, "killall"},
	{from_gm_recall_, "recall"},
	{from_gm_summon_, "summon"},
	{from_gm_item_monster_, "itemmonster"},
	{from_gm_remove_, "remove"},
	{from_gm_shift_, "shift"},
	{from_gm_change_map_type_, "changemaptype"},
	{from_gm_rc_, "rc"},
	{from_sn_doridori_, "sndoridori"},
	{from_sn_explosion_spirits_, "snexplosionspirits"},
	{from_pm_ignore_, "wisexin"},
	{from_pm_ignore_list_, "wisexlist"},
	{from_pm_ignore_all_, "wisall"},
	{from_friends_list_add_, "friendslistadd"},
	{from_friends_list_remove_, "friendslistremove"},
	{from_friends_list_reply_, "friendslistreply"},
	{from_black_smith_, "blacksmith"},
	{from_alchemist_, "alchemist"},
	{from_taekwon_, "taekwon"},
	{from_ranking_pk_, "rankingpk"},
	{from_feel_save_ok_, "feelsaveok"},
	{from_debug_, "debug"},
	{from_change_hom_unculus_name_, "changehomunculusname"},
	{from_hom_move_to_master_, "hommovetomaster"},
	{from_hom_move_to_, "hommoveto"},
	{from_hom_attack_, "homattack"},
	{from_hom_menu_, "hommenu"},
	{from_storage_password_, "storagepassword"},
	{from_hotkey_, "hotkey"},
	{from_auto_revive_, "autorevive"},
	{from_check_, "check"},
	{from_adopt_request_, "adoptrequest"},
	{from_adopt_reply_, "adoptreply"},
	// MAIL SYSTEM
	{from_mail_refresh_, "mailrefresh"},
	{from_mail_read_, "mailread"},
	{from_mail_get_attach_, "mailgetattach"},
	{from_mail_delete_, "maildelete"},
	{from_mail_return_, "mailreturn"},
	{from_mail_set_attach_, "mailsetattach"},
	{from_mail_win_open_, "mailwinopen"},
	{from_mail_send_, "mailsend"},
	// AUcTION SYSTEM
	{from_auction_search_, "auctionsearch"},
	{from_auction_buy_sell_, "auctionbuysell"},
	{from_auction_set_item_, "auctionsetitem"},
	{from_auction_cancel_reg_, "auctioncancelreg"},
	{from_auction_register_, "auctionregister"},
	{from_auction_cancel_, "auctioncancel"},
	{from_auction_close_, "auctionclose"},
	{from_auction_bid_, "auctionbid"},
	{from_cash_shop_buy_, "cashshopbuy"},
	{from_view_player_equip_, "viewplayerequip"},
	{from_equip_tick_box_, "equiptickbox"},
	{from_battle_chat_, "battlechat"},
	{from_mer_menu_, "mermenu"},
	{NULL, NULL},
};

void FromDispatch::init()
{
	::memset(_functions, 0, sizeof(FromFunctionPtr) * MAX_FROM_INDEX);
	::memset(_packet_size, 0, sizeof(size_t) * MAX_FROM_INDEX);
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
					//printf("Message entry has registered! Index(Hex): %s, \t Function: %s\n", fields[0]._val.c_str(), fields[2]._val.c_str());
				}
				else
				{
					printf("Message entry has no process function! Index(Hex): %s, \t Function: %s\n", fields[0]._val.c_str(), fields[2]._val.c_str());
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
			_packet_size[index] = conversion_cast<size_t>(fields[1]._val);
		}
	}while(db.next());
}
