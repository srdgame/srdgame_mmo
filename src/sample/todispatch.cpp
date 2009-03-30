#include "todispatch.h"
#include "login.h"
#include "char.h"
#include "share.h"
#include "opcode.h"
#include "packetdb.h"
#include "conversion_cast.h"

using namespace ro;
using namespace std;
using namespace srdgame;
using namespace srdgame::opcode;

// Now we are using the same packet db file.
#define TO_DB_FILE_NAME "./data/db/from_packet_db.txt"

static struct {
	ToFunctionPtr ptr;
	int op;
} to_parse_func[]={
	{to_send_0x006a_, ES_DISCONNECT},
	{to_send_0x0283_, ES_CONNECT_TO_MAP},
	{NULL, 0},
};

void ToDispatch::init()
{
	::memset(_functions, 0, sizeof(ToFunctionPtr) * MAX_TO_INDEX);
	size_t i = 0;
	while (to_parse_func[i].ptr && to_parse_func[i].op)
	{
		_functions[to_parse_func[i].op] = to_parse_func[i].ptr;
		++i;
	}
	PacketDB db(TO_DB_FILE_NAME);
	//printf("Start to load the packet db\n");
	do
	{
		std::vector<DBField>& fields = db.fetch();	
		if (fields.size() >= 2)
		{
			int index = hex2int(fields[0]._val);
			_packet_size[index] = conversion_cast<size_t>(fields[1]._val);
		}
	}while(db.next());

}
