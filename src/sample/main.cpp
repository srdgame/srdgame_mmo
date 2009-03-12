
#include "defs.h"
#include <iostream>

size_t from_stream(Packet* dest, const char* src, size_t size)
{
	std::cout << "In from_stream function!!!!" << std::endl;
	return 0;
}
size_t to_stream(char* dest, const Packet* src)
{	
	std::cout << "In to_stream function!!!!" << std::endl;
	return 0;
}

