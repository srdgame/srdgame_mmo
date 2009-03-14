#include "packetdump.h"
#include "autolock.h"
#include "log.h"
#include "packetparser.h"
#include <cstring>

using namespace srdgame;
using namespace std;

#define DUMP_S_FILE_NAME "packet_s.dump"
#define DUMP_P_FILE_NAME "packet_p.dump"

PacketDump::PacketDump() : _inited(false)
{
	_file_s_lock.lock();
	_file_p_lock.lock();
	_file_s.open(DUMP_S_FILE_NAME, ios::app | ios::out);
	_file_p.open(DUMP_S_FILE_NAME, ios::app | ios::out);
	if (!_file_s.good())
	{
		LogError("PacketDump", "Could not open dump file: %s", DUMP_S_FILE_NAME);
	}
	if (!_file_p.good())
	{
		LogError("PacketDump", "Could not open dump file: %s", DUMP_P_FILE_NAME);
	}
	_inited = _file_s.good() && _file_p.good();
	if (!_inited)
	{
		_file_s.close();
		_file_p.close();
	}
	_file_s_lock.unlock();
	_file_p_lock.unlock();
}
PacketDump::~PacketDump()
{
	if (_inited)
	{
		_file_s.close();
		_file_p.close();
	}
	_inited = false;
}
void PacketDump::dump(const char* source, const char* data, size_t len)
{
	AutoLock lock(_file_s_lock);
	if (!_inited || !_file_s.good())
		return;
	dump_to_file(_file_s, source, data, len);
}

void PacketDump::dump(const char* source, const Packet& data)
{
	size_t size = PacketParser::get_ex_len(data);
	if (size <= 0)
		return;

	AutoLock lock(_file_p_lock);
	if (!_inited || !_file_p.good())
		return;

	char* buf = new char[size];
	memcpy(buf, data.param.Data, size);
	dump_to_file(_file_p, source, buf, size);
}

char PacketDump::to_char(int i)
{
	return (i <= 9) ? '0' + i : 'A' + (i - 10);
}
int PacketDump::to_int(char c)
{
	return (c > '9') ? c - 'A' + 10 : c - '0';
}

void PacketDump::dump_to_file(fstream& file, const char* source, const char* data, size_t len)
{
	const int char_offset = 16*3 + 2;
	const int line_size = 16*3 + 16 + 3;
	char line[line_size + 1];

	if (file.good())
	
	file << "FROM: N " <<  source << std::endl;

	file << "OFFSET  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | 0123456789ABCDEF" << std::endl;
	file << "--------------------------------------------------------------------------" << std::endl;

	size_t i = 0;
	while (i < len)
	{
		int bi = 0;
		int ci = 0;

		int start_i = i;
		for (int line_i = 0; i < len && line_i < 16; i++, line_i++)
		{
			line[bi++] = to_char(*data >> 4);
			line[bi++] = to_char(*data & 0x0f);
			line[bi++] = ' ';
			line[char_offset + (ci++)] = (isprint(*data) ? *data : '.');
			data++;
		}

		while (bi < 16 * 3)
		{
			line[bi++] = ' ';
		}

		line[char_offset + (ci++)] = '\n';
		line[char_offset + ci] = 0;

		char s_line[line_size + 1 + 8];
		sprintf(s_line, "%06X  %s", start_i, line);
		file << s_line;
	}
	file << std::endl;
	file << std::endl;
}

