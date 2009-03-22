#ifndef RO_SQL_H_
#define RO_SQL_H_

#include "ro_defs.h"
#include "userinfo.h"
#include "metadefs.h"
#include "ro.h"
#include "rocharinfo.h"
#include "field.h"

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
	size_t load_chars(int account_id, RoCharInfo*& result);
	bool load_char(int char_id, RoCharInfo& info, bool load_everything = false);
	bool save_char(int char_id, RoCharInfo& info);
protected:
	bool fetch_account_info(srdgame::Field* fields, srdgame::UserInfoEx< RoUser >& info);
	bool fetch_chars_info(srdgame::Field* fields, RoCharInfo& info);
	bool fetch_char_info(srdgame::Field* fields, RoCharInfo& info);
};
}

#endif
