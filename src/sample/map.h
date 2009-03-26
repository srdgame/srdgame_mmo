#ifndef RO_MAP_H_
#define RO_MAP_H_

#include <cstddef>
#include "defs.h"

// 
size_t from_map_connect(Packet* dest, const char* src, size_t size);
size_t to_map_connect(char* buf, const Packet* packet);

#endif
