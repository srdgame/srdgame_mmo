#ifndef MAP_CONF_H_
#define MAP_CONF_H_

#include <vector>
#include <string>

namespace srdgame
{
class MapConf
{
public:
	MapConf(const char* fn);
	virtual ~MapConf();

public:
	bool load();
	inline std::vector<std::string>& get_maps()
	{
		return _maps;
	}
protected:
	std::vector<std::string> _maps;
	std::string _fn;
};
}

#endif
