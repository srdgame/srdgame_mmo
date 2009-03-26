#ifndef SHARE_H_
#define SHARE_H_
#include "defs.h"
namespace ro
{
// 5: Your game's exe file is not the lastest version.
// 3: Reject by server.
size_t send_6a_to_disconnect(char* buf, uint8 reason);
}

#endif
