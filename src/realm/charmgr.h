#ifndef CHAR_MGR_H_
#define CHAR_MGR_H_

#include "singleton.h"
//#include "mutex.h"

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

	size_t load_chars(int account_id, ro::RoCharInfo* chars);
private:
	ro::RoSql* _sql;
};
}

#endif
