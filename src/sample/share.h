#ifndef SHARE_H_
#define SHARE_H_
#include "defs.h"

#define TO_DC(x) size_t to_send_##x(char* buf, const Packet* packet)

namespace ro
{
size_t send_account(char* buf, const Packet* packet);
// 5: Your game's exe file is not the lastest version.
// 3: Reject by server.
//size_t to_send_0x006a(char* buf, const Packet* packet);
TO_DC(0x006a);
size_t send_6a_to_disconnect(char* buf, uint8 reason);
TO_DC(0x0283);
TO_DC(0x0073); // map auth ok.
TO_DC(0x008e);
TO_DC(0x017f);
TO_DC(0x0097);

TO_DC(0x01d7); // send the look update.

}

#endif
