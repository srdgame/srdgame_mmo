#include "singleton.h"
#include <string>

using srdgame::Singleton;
namespace srdgame
{
class ConfigFile;
};

namespace ro
{
class RoConf : public Singleton< RoConf >
{
public:
	RoConf();
	~RoConf();

public:
	std::string get_data_path();
	void load();
private:
	srdgame::ConfigFile* _config;
	bool _loaded;
};
}
