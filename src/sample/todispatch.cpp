#include "todispatch.h"
#include "login.h"
#include "char.h"
#include "map.h"
#include "packetdb.h"
#include "conversion_cast.h"

using namespace ro;
using namespace std;

#define TO_DB_FILE_NAME "./data/db/to_packet_db.txt"

static struct {
	ToFunctionPtr ptr;
	const char *name;
} to_parse_func[]={
	//{from_userinfo,					"wanttologin"},
	{NULL, NULL},
};

void ToDispatch::init()
{
	::memset(_functions, 0, sizeof(ToFunctionPtr) * MAX_TO_INDEX);
	::memset(_packet_size, 0, sizeof(size_t) * MAX_TO_INDEX);
	size_t i = 0;
	while (to_parse_func[i].ptr && to_parse_func[i].name)
	{
		_string_to_function.insert(pair<std::string, ToFunctionPtr>(string(to_parse_func[i].name), to_parse_func[i].ptr));
		++i;
	}
	PacketDB db(TO_DB_FILE_NAME);
	printf("Start to load the packet db\n");
	do
	{
		std::vector<DBField>& fields = db.fetch();
		if (fields.size() > 2)
		{
			int index = hex2int(fields[0]._val);
		//	printf("Process Entry : index : %d s : %s \n", index, fields[0]._val.c_str());
			if (index <= MAX_TO_INDEX)
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
