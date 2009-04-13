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
	{to_send_0x006a, ES_DISCONNECT},
	{to_send_0x0283, ES_CONNECT_TO_MAP},
	{to_send_0x0073, ES_MAP_AUTH},
	{to_send_0x008e, ES_MESSAGE},
	{to_send_0x017f, ES_PRIVATE_MSG},
	{to_send_0x0097, ES_WIS_MESSAGE},
	{to_send_0x01d7, ES_LOOK_UPDATE},
	{to_send_0x0201, ES_FRIEND_LIST},
	{to_send_0x0206, ES_FRIEND_STATUS},
	{to_send_0x00a4, ES_EQUIP_ITEM_LIST},
	{to_send_0x01ee, ES_STACKABLE_ITEM_LIST},
	{to_send_0x0087, ES_WALK_TO},
	{to_send_0x007f, ES_TICK_COUNT},
	{to_send_0x0086, ES_UNIT_MOVE},
	{to_send_0x00aa, ES_EQUIP_ITEM},
	{to_send_0x00ac, ES_UNEQUIP_ITEM},
	{to_send_0x00a0, ES_ADD_ITEM},
	{to_send_0x008a, ES_UNIT_ACTION},
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
			_packet_size[index] = conversion_cast<int>(fields[1]._val);
			// debug
			if (index == 243)
				printf("TO Message entry has registered! Index(Hex): %s,  \t paccket size : % d\n", fields[0]._val.c_str(), conversion_cast<int>(fields[1]._val));

		}
	}while(db.next());

}
