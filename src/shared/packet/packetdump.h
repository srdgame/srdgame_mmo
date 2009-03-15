#include "packetdefs.h"
#include "singleton.h"
#include "mutex.h"

#include <ctype.h>
#include <cstdlib>
#include <cstdio>

namespace srdgame
{
class PacketDump : public Singleton < PacketDump >
{
public:
	PacketDump();
	~PacketDump();
	
	// dump the source data.
	void dump(const char* source, const char* data, size_t len);

	// dump our packet data.
	void dump(const char* source, const Packet& data);
protected:
	// So far we do not think we will dump it when server is running.
	void start_worker();

	// Helper functions
	char to_char(int i);
	int to_int(char c);

	void dump_to_file(FILE* file, const char* source, const char* data, size_t len);
private:
	bool _inited;
	Mutex _file_s_lock;
	FILE* _file_s;
	Mutex _file_p_lock;
	FILE* _file_p;
};
}
