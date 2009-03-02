/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketDefines.h - Any platform-specific defines/includes go here.
 *
 */

#ifndef SOCKET_DEFINES_H
#define SOCKET_DEFINES_H

// -_-! I have to include this for memset();
#include <cstring>

#define UNIX_FLAVOUR_LIUNX 9

// 设置你的自己的系统
#define UNIX_FLAVOUR UNIX_FLAVOUR_LINUX

using namespace std;

/* Implementation Selection */
#ifdef WIN32		// Easy
#define CONFIG_USE_IOCP
#define CONFIG_USE_SELECT
#else

// unix defines
#define SOCKET int
#define SD_BOTH SHUT_RDWR

#if UNIX_FLAVOUR == UNIX_FLAVOUR_LINUX

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

// select: epoll
#include <sys/epoll.h>
#define CONFIG_USE_EPOLL

#elif UNIX_FLAVOUR == UNIX_FLAVOUR_BSD

// select: kqueue
#include <sys/event.h>
#define CONFIG_USE_KQUEUE

#elif UNIX_FLAVOUR == UNIX_FLAVOUR_OSX
// select: kqueue
#include <sys/event.h>
#define CONFIG_USE_KQUEUE


#endif

#endif

/* IOCP Defines */

#ifdef CONFIG_USE_IOCP

#endif

#endif
