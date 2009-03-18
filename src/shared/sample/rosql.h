#ifndef RO_SQL_H_
#define RO_SQL_H_

#include "ro_defs.h"
#include "userinfo.h"
#include "metadefs.h"
#include "rouser.h"
#include "field.h"

namespace ro
{
class RoSql
{
public:
	RoSql();
	virtual ~RoSql();

	bool create_account(srdgame::UserInfoEx<RoUser>& info);
	srdgame::uint32 get_max_id();

	bool load_account(uint32 id, srdgame::UserInfoEx<RoUser>& info);
	bool load_account(const char* name, srdgame::UserInfoEx<RoUser>& info);

protected:
	bool fetch_account_info(Field* fields, srdgame::UserInfoEx< RoUser >& info);
};
}

#endif
