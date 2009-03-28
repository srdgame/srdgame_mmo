#ifndef PACKET_DB_H_
#define PACKET_DB_H_

#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

namespace ro
{
inline int hex2int(std::string s)
{
	//s = s.substr(s.find('x') + 1);
	int v;
	std::istringstream iss(s);
	iss >> std::setbase(0) >> v;
	return v;
}
struct DBField
{	
	std::string _val;
};
class PacketDB
{
public:
	PacketDB(const char* fn);
	virtual ~PacketDB();

	std::vector<DBField>& fetch();
	bool next();

private:
	std::fstream _file;
	char _buf[512];
	std::vector<DBField> _fields;
};
}

#endif
