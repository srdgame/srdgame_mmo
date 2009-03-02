// Lisence: GPL
// autoconfig.cpp
//
#include <iostream>
#include <fstream>
#include "config.h"
#include "conversion_cast.h"

using namespace std;
using namespace srdgame;

Config::Config(std::string& fn)
{
	if (-1 == load_file(fn))
	{
		std::cout << "ERROR for conf file: " << fn << std::endl;
	}
}

Config::~Config()
{
}


int Config::load_file(std::string& fn)
{
	fstream file(fn.c_str());
//	if (!file.good())
//		return -1;

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
}
