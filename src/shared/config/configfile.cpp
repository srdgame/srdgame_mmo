// Lisence: GPL
// autoconfig.cpp
//
#include <iostream>
#include <fstream>
#include "configfile.h"
#include "conversion_cast.h"

using namespace std;
using namespace srdgame;

ConfigFile::ConfigFile(std::string& fn) : _loaded (false)
{
	if (0 == load_file(fn))
	{
		_loaded = false;
		std::cout << "ERROR for conf file: " << fn << std::endl;
	}
	else
	{
		_loaded = true;
	}
}
ConfigFile::ConfigFile() : _loaded (false)
{
}

ConfigFile::~ConfigFile()
{
	_loaded = false;
}


int ConfigFile::load_file(std::string& fn)
{
	fstream file(fn.c_str());
	if (!file.good())
		return -1;

	string buf;
	while (getline(file, buf)) {
		if (buf.empty())
			continue;
		if (buf[0] == '#')
			continue;
		int index = buf.find("=");
		if (index == buf.npos)
			continue;

		string key = buf.substr(0, index);
		string val = buf.substr(index + 1);
		//std::cout << "key : " << key << " val: " << val << std::endl;
		_attrs.insert(make_pair< string, string>(key, val));
	}
	return 0;
}
