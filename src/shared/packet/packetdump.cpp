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
	_file_s = fopen(DUMP_S_FILE_NAME, "a");
	_file_p = fopen(DUMP_P_FILE_NAME, "a");
	if (!_file_s)
	{
		LogError("PacketDump", "Could not open dump file: %s", DUMP_S_FILE_NAME);
	}
	if (!_file_p)
	{
		LogError("PacketDump", "Could not open dump file: %s", DUMP_P_FILE_NAME);
	}
	_inited = _file_s && _file_p;
	if (!_inited)
	{
		fclose(_file_s);
		fclose(_file_p);
	}
	_file_s_lock.unlock();
	_file_p_lock.unlock();
}
PacketDump::~PacketDump()
{
	if (_inited)
	{
		fclose(_file_s);
		fclose(_file_p);

	}
	_inited = false;
}
void PacketDump::dump(const char* source, const char* data, size_t len)
{
	AutoLock lock(_file_s_lock);
	if (!_inited || !_file_s)
		return;
	dump_to_file(_file_s, source, data, len);
}

void PacketDump::dump(const char* source, const Packet& data)
{
	size_t size = PacketParser::get_ex_len(data);
	if (size <= 0)
		return;

	AutoLock lock(_file_p_lock);
	if (!_inited || !_file_p)
		return;

	//char* buf = new char[size];
	//memcpy(buf, data.param.Data, size);
	dump_to_file(_file_p, source, data.param.Data, size);
}

char PacketDump::to_char(int i)
{
	return (i <= 9) ? '0' + i : 'A' + (i - 10);
}
int PacketDump::to_int(char c)
{
	return (c > '9') ? c - 'A' + 10 : c - '0';
}

void PacketDump::dump_to_file(FILE* file, const char* source, const char* data, size_t len)
{
	const int char_offset = 16*3 + 2;
	const int line_size = 16*3 + 16 + 3;
	char line[line_size + 1];
	
	fprintf(file, "FROM: | %s \n", source);

	fprintf(file, "OFFSET  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | 0123456789ABCDEF\n");
	fprintf(file, "--------------------------------------------------------------------------\n");

	line[char_offset - 1] = ' ';
	line[char_offset - 2] = ' ';
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

		fprintf(file, "%06X  %s", start_i, line);	
	}
	fflush(file);
	fprintf(file, "\n\n");
}

