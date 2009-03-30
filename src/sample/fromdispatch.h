#ifndef FROM_DISPATCH_H_
#define FROM_DISPATCH_H_

#include "singleton.h"
#include "defs.h"
#include <cstring>
#include <string>
#include <map>

#define MAX_FROM_INDEX 1024

namespace ro
{
struct FromStruct
{
	FromStruct(Packet* p, const char* buf, size_t size) : _packet(p), _buf(buf), _size(size)
	{
	}
	Packet* _packet;
	const char* _buf;
	size_t _size;
};
typedef size_t (*FromFunctionPtr)(Packet* packet, const char* buf, size_t);

class FromDispatch : public srdgame::Singleton <FromDispatch>
{
public:
	FromDispatch()
	{
		::memset(_functions, 0, sizeof(FromFunctionPtr) * MAX_FROM_INDEX);
		::memset(_packet_size, 0, sizeof(size_t) * MAX_FROM_INDEX);
	};
	~FromDispatch()
	{
	}

	void init();
	
	inline size_t call(unsigned int op, FromStruct& s)
	{
		FromFunctionPtr ptr = *(_functions + op);
		if (!ptr)
		{
			printf("Could not loopup the function for op : %d", op);
			return _packet_size[op];
		}
		printf("Calling the registered entry, OP : %d", op);
		size_t res = (*ptr)(s._packet, s._buf, s._size);
		if (res != _packet_size[op])
		{
			printf("================================================================================\n");
			printf("================= return value is not same as packet db, res : %d, db : %d", (int)res, (int)_packet_size[op]);
			printf("================================================================================\n");
		}
		return _packet_size[op];
	}
private:
	size_t default_call(Packet* packet, const char* buf, size_t size){return 0;}
	FromFunctionPtr _functions[MAX_FROM_INDEX];
	std::map<std::string, FromFunctionPtr> _string_to_function;
	size_t _packet_size[MAX_FROM_INDEX];
};
}

#endif
