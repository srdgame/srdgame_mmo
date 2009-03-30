#include "romap.h"
#include "log.h"
#include <cstring>
#include <cassert>
#include "strlib.h"

#include "grfio.h"

using namespace ro;
using namespace std;
using namespace srdgame;

// gatn
static MapCell map_gat2cell(int gat)
{
	struct MapCell cell;
	memset(&cell, 0, sizeof(cell));

	switch( gat )
	{
	case 0: cell.walkable = 1; cell.shootable = 1; cell.water = 0; break; // walkable ground
	case 1: cell.walkable = 0; cell.shootable = 0; cell.water = 0; break; // non-walkable ground
	case 2: cell.walkable = 1; cell.shootable = 1; cell.water = 0; break; // ???
	case 3: cell.walkable = 1; cell.shootable = 1; cell.water = 1; break; // walkable water
	case 4: cell.walkable = 1; cell.shootable = 1; cell.water = 0; break; // ???
	case 5: cell.walkable = 0; cell.shootable = 1; cell.water = 0; break; // gap (snipable)
	case 6: cell.walkable = 1; cell.shootable = 1; cell.water = 0; break; // ???
	default:
		LogWarning("map_gat2cell", "unrecognized gat type '%d'\n", gat);
		break;
	}

	return cell;
}

static int map_cell2gat(const MapCell& cell)
{
	if( cell.walkable == 1 && cell.shootable == 1 && cell.water == 0 ) return 0;
	if( cell.walkable == 0 && cell.shootable == 0 && cell.water == 0 ) return 1;
	if( cell.walkable == 1 && cell.shootable == 1 && cell.water == 1 ) return 3;
	if( cell.walkable == 0 && cell.shootable == 1 && cell.water == 0 ) return 5;

	LogWarning("map_cell2gat", "cell has no matching gat type\n");
	return 1; // default to 'wall'
}


RoMap::RoMap() : _loaded(false)
{
}

RoMap::~RoMap()
{
	unload();
}

bool RoMap::load(const char* fn)
{
	unload();
	_file.open(fn, ios::binary | ios::in);
	if (_file.bad())
	{
		LogError("RO_MAP", "Could not open map cache file : %s", fn);
		return false;
	}
	_file.read((char*)&_header, sizeof(_header));
	if (_file.bad())
	{
		_file.close();
		LogError("RO_MAP", "Incorrect map cache file : %s", fn);
		return false;
	}
	LogNotice("RO_MAP", "Start to process map cache, map cout : %d, \t file size : %d", _header._map_count, _header._file_size);
	_loaded = true;
	int i = 0;
	_maps.resize(_header._map_count);
	for (; i < _header._map_count; ++i)
	{
		MapCacheMapInfo info;
		memset(&info, 0, sizeof(info));
		_file.read((char*)&info, sizeof(info));
		if (_file.bad())
		{
			LogError("RO_MAP", "Error in reading map cache file");
			unload();
			return false;
		}
		size_t len = (unsigned long)info._xs * (unsigned long)info._ys * 2;
	//	LogDebug("RO_MAP", "Loaded map with name: %s, len: %d", info._name, info._len);
		_maps.push_back(info);

		// Jump to next map info;
		_file.seekg(info._len, ios::cur);
	}
	LogSuccess("RO_MAP", "Load map cache sucessfuly!!");
}

void RoMap::unload()
{
	if(_loaded)
	{
		_loaded = false;
		_maps.clear();
		_file.close();
		::memset(&_header, 0, sizeof(_header));
	}
}

int RoMap::get_map(const char* name, MapData& data)
{
	::memset(&data, 0, sizeof(MapData));

	if (!is_loaded())
		return -1;
	size_t i = 0;
	size_t off = sizeof(_header);
	for (; i < _maps.size(); ++i)
	{
		if (0 == strcmp(_maps[i]._name, name))
			break;
		off += _maps[i]._len;
	}
	if (i >= _maps.size())
		return -1;

	// Now it is time to load the file.
	off += sizeof(MapCacheMapInfo);
	if (off >= _header._file_size)
		return -1;
	//LogDebug("RO_MAP", "Load map data from offset: %d \t len: %d", off, _maps[i]._len);
	
	_file.seekg(off, ios::beg);
	unsigned char *buf = new unsigned char[_maps[i]._len];
	memset(buf, 0, _maps[i]._len);
	unsigned long size = (unsigned long)_maps[i]._xs * (unsigned long)_maps[i]._ys;
	unsigned char *buf2 = new unsigned char[size];
	memset(buf2, 0, size);
	data._cells = new MapCell[size];
	memset(data._cells, 0, sizeof(MapCell) * size);

	_file.read((char*)buf, _maps[i]._len);
	if (_file.bad())
	{
		assert(false);
	}
	// debug
	//
	/*
	char ch[64];
	bin2hex(ch, buf, 8);
	LogDebug("RO_MAP", "Load map data from %s ....", ch);
	*/
	//
	//
	//
	decode_zip(buf2, &size, buf, _maps[i]._len);

	for (int xy = 0; xy < size; ++xy)
	{
		data._cells[xy] = map_gat2cell(buf2[xy]);
	}
	delete [] buf;
	delete [] buf2;
	return i;
}
