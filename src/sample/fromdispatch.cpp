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
	{from_userinfo,					"wanttologin"},
	{from_get_loginserver_info, 	"getloginserverinfo"},
	{from_admin_login, 				"adminwanttologin"},
	{from_login_to_char,			"connecttochar"},
	{from_select_char,				"selectchar"},
	{from_create_char, 				"createchar"},
	{from_delete_char,				"deletechar"},
	{from_keep_alive,				"keepalive"},
	{from_char_rename,				"renamechar"},
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
	PacketDB db(FROM_DB_FILE_NAME);
	printf("Start to load the packet db\n");
	do
	{
		std::vector<DBField>& fields = db.fetch();
		if (fields.size() > 2)
		{
			int index = hex2int(fields[0]._val);
		//	printf("Process Entry : index : %d s : %s \n", index, fields[0]._val.c_str());
			if (index <= MAX_FROM_INDEX)
			{
				_functions[index] = _string_to_function[fields[3]._val];
				if (_functions[index])
				{
					printf("Message entry has registered! Index : %d \t Hex : %s", index, fields[0]._val.c_str());
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
