#ifndef TO_DISPATCH_H_
#define TO_DISPATCH_H_

#include "singleton.h"
#include "defs.h"
#include "dispatch.h"
#include "singleton.h"
#include "defs.h"
#include <cstring>
#include <string>
#include <map>

#define MAX_TO_INDEX 1024

namespace ro
{
struct ToStruct
{
	ToStruct(char* buf, const Packet* packet) : _buf(buf), _packet(packet)
	{
	}
	char* _buf;
	const Packet* _packet;
};
typedef size_t (*ToFunctionPtr)(char* buf, const Packet* packet);

class ToDispatch : public srdgame::Singleton <ToDispatch>
{
public:
	ToDispatch()
	{
		::memset(_functions, 0, sizeof(ToFunctionPtr) * MAX_TO_INDEX);
		::memset(_packet_size, 0, sizeof(int) * MAX_TO_INDEX);
	};
	~ToDispatch()
	{
	}

	void init();
	
	inline size_t call(unsigned int op, ToStruct& s)
	{
		ToFunctionPtr ptr = *(_functions + op);
		if (!ptr)
		{
			printf("No function stands for op : %d", op);
			return 0;
		}
		printf("Calling the registered entry, OP : %d\n", op);
		size_t size =  (*ptr)(s._buf, s._packet);
		op = PUINT16(s._buf, 0);
		printf("Converted one packet with op: %d\n", op);
		if (size != _packet_size[op])
		{			
			if (_packet_size[op] == -1)
			{
				//printf("-1 has been found, use the return value : %d", (int)(res? res : s._size));
				return size;
			}
			else
			{
				printf("================================================================================\n");
				printf("================= return value is not same as packet db, res : %d, db : %d\n", (int)size, _packet_size[op]);
				printf("================================================================================\n");
			}
		}
		return _packet_size[op];
	}
private:
	size_t default_call(char* buf, Packet* p){return 0;}

	ToFunctionPtr _functions[MAX_TO_INDEX];
	int _packet_size[MAX_TO_INDEX];
};
}

#endif
