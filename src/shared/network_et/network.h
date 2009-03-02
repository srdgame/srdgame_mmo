/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * Network.h - Include this file in your .cpp files to gain access
 *			 to features offered by this library.
 *
 */

#ifndef NETWORK_H_
#define NETWORK_H_

//#include "../Log.h"
//#include "../NGLog.h"
#include "bipbuffer.h"
#include "socketdefs.h"
#include "socketops.h"
#include "socket.h"

#ifdef CONFIG_USE_IOCP
#include "socketmgr_Win32.h"
#include "listensocket_Win32.h"
#endif

#ifdef CONFIG_USE_EPOLL
#include "socketmgr_Linux.h"
#include "listensocket_Linux.h"
#endif

#ifdef CONFIG_USE_KQUEUE
#include "socketmgr_FreeBSD.h"
#include "listensocket_FreeBSD.h"
#endif

#endif
