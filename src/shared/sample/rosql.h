#ifndef RO_SQL_H_
#define RO_SQL_H_

#include "ro_defs.h"
#include "userinfo.h"
#include "rouser.h"
#include "metadefs.h"
#include "rocharinfo.h"
#include "field.h"
#include <vector>

namespace ro
{
class RoSql
{
public:
	RoSql();
	virtual ~RoSql();

	bool create_account(srdgame::UserInfoEx<RoUser>& info);
	int get_max_id();

	bool load_account(int id, srdgame::UserInfoEx<RoUser>& info);
	bool load_account(const char* name, srdgame::UserInfoEx<RoUser>& info);

	int get_max_char_id();
	size_t load_chars(int account_id, RoCharInfoBase*& result);
	bool load_char(int char_id, RoCharInfo& info, bool load_everything = false);
	bool save_char(int char_id, RoCharInfo& info);

	bool load_items(int char_id, std::vector<RoCharItem>& items);
	bool save_items(int char_id, const std::vector<RoCharItem>& items);

	bool load_mem_data(int char_id, ...);// TODO:
	bool save_mem_data(int char_id, ...);

	bool load_cart(int char_id, std::vector<RoCharItem>& cart_items); //TODO:
	bool save_cart(int char_id, const std::vector<RoCharItem>& cart_items);

	bool load_storage(int char_id, ...);
	bool save_storage(int char_id, ...);

	bool load_skill(int char_id, std::vector<RoCharSkillInfo>& skills);
	bool save_skill(int char_id, const std::vector<RoCharSkillInfo>& skills);

	bool load_friends(int char_id, std::vector<RoCharFriendInfo>& friends);
	bool save_friends(int char_id, const std::vector<RoCharFriendInfo>& friends);

	bool load_hotkey(int char_id, std::vector<RoCharHotKey>& keys);
	bool save_hotkey(int char_id, const std::vector<RoCharHotKey>& keys);
protected:
	bool fetch_account_info(srdgame::Field* fields, srdgame::UserInfoEx< RoUser >& info);
	bool fetch_chars_info(srdgame::Field* fields, RoCharInfoBase& info);
	bool fetch_char_info(srdgame::Field* fields, RoCharInfo& info);
};
}

#endif
