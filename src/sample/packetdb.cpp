#include "packetdb.h"
#include <cstring>
#include <string>
#include <cassert>
#include <cstdlib>
#include "conversion_cast.h"

using namespace ro;
using namespace std;
using namespace srdgame;

PacketDB::PacketDB(const char* fn) : _file(fn)
{
	printf("Start to load packet db: %s", fn);
	if (_file.bad())
	{
		printf("Could not read the packet db file : %s", fn);
		assert(false);
		exit(0);
	}
	next();// fetch the first line
}
PacketDB::~PacketDB()
{
	_fields.clear();
	_file.close();
}

std::vector<DBField>& PacketDB::fetch()
{
	return _fields;
}

bool PacketDB::next()
{
	//printf("IN NExt!!!!!!!!!!!!!!!!!!\n");
	memset(_buf, 0, 512);
	_fields.clear();
	_file.getline(_buf, 512);
	if (!_file.good())
	{
		printf("FINISHED\n\n");
		return false;
	}


	std::string temp(_buf);

	if (temp.empty() || temp[0] == '/' | temp[1] == '/')
		return true;

	/*std::string sub;
	size_t i = temp.find(',', i);
	size_t o_i = 0;
	if (i == std::string::npos)
		return true;

	while (i != std::string::npos)
	{

		DBField f;
		f._val = temp.substr(o_i, i - o_i);
		o_i = ++i;
		printf("Sub is %s index : %d \n", f._val.c_str(), i);
		_fields.push_back(f);
		i = temp.find(',', i);

	}*/
	size_t i = 0;
	size_t j = 0;
	while (i != std::string::npos)
	{
		i = temp.find(',', i);
		DBField f;
		f._val = temp.substr(j, i - j);
	//	printf("Sub is %s index : %d \n", f._val.c_str(), i);
		_fields.push_back(f);
		if (i == std::string::npos)
			break;
		j = ++i;
	}
	return true;
}
