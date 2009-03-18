// Lisence:GPL
// config.h
//
//
#ifndef AUTOCONFIG_H_
#define AUTOCONFIG_H_

#include <string>
#include <map>
#include "conversion_cast.h"

namespace srdgame 
{
class ConfigFile
{
public:
	ConfigFile(const std::string& fn);
	ConfigFile();
	virtual ~ConfigFile();

	template<class VT>
	VT get_value(const std::string& name)
	{
		return conversion_cast<VT, std::string>(get_value(name));
	}
	// more?
	//template<>
	std::string get_value(const std::string& name)
	{
		return _attrs[name];
	}
	int load_file(const std::string& fn);

	bool is_loaded() { return _loaded;}
protected:
	std::map<std::string, std::string> _attrs;
	bool _loaded;
};
}

#endif
