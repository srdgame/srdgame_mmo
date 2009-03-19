#include "roconf.h"
#include "configfile.h"


using namespace srdgame;
using namespace ro;
using namespace std;

#define RO_CONF_FILE "ro.conf"

RoConf::RoConf() : _config(NULL), _loaded(false)
{
}

RoConf::~RoConf()
{
	if (_config)
	{
		delete _config;
	}
}

string RoConf::get_data_path()
{
	if (_loaded)
	{
		if (_config && _config->is_loaded())
		{
			return _config->get_value<string>("DATA_PATH");
		}
	}
	return "";
}

void RoConf::load()
{
	if(_loaded)
	{
		return;
	}
	_loaded = true;
	_config = new ConfigFile(RO_CONF_FILE);
}

