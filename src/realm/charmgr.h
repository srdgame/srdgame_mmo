#ifndef CHAR_MGR_H_
#define CHAR_MGR_H_

#include "singleton.h"
#include <string>
#include "mutex.h"
#include "configfile.h"

namespace ro
{
struct CreateCharData;
struct RoCharInfo;
struct RoSql;
};

namespace srdgame
{
class CharMgr : public Singleton < CharMgr >
{
public:
	CharMgr();
	~CharMgr();

	ro::RoCharInfo* create_new(ro::CreateCharData* data, int account_id);

	size_t load_chars(int account_id, ro::RoCharInfo*& chars);

	bool load_char_detail(int char_id, ro::RoCharInfo& info);
private:
	ro::RoSql* _sql;
	ConfigFile _config;
};
}

#endif
