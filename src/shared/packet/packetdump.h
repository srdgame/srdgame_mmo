#include "packetdefs.h"
#include "singleton.h"
#include "mutex.h"

#include <fstream>

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
	int from_char(char c);

	void dump_to_file(std::fstream& file, const char* source, const char* data, size_t len);
private:
	bool _inited;
	Mutex _file_s_lock;
	std::fstream _file_s;
	Mutex _file_p_lock;
	std::fstream _file_p;
};
}
