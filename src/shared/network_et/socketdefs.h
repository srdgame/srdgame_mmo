/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketDefines.h - Any platform-specific defines/includes go here.
 *
 */

#ifndef SOCKET_DEFINES_H
#define SOCKET_DEFINES_H


// 设置你的自己的系统
#define UNIX_FLAVOUR UNIX_FLAVOUR_LINUX

using namespace std;

/* Implementation Selection */
#ifdef WIN32		// Easy
#define CONFIG_USE_IOCP
//#define CONFIG_USE_SELECT
#else

// unix defines
#define SOCKET int
#define SD_BOTH SHUT_RDWR

#if UNIX_FLAVOUR == UNIX_FLAVOUR_LINUX
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
enum SocketIOEvent
{
	SOCKET_IO_EVENT_READ_COMPLETE   	= 0,
	SOCKET_IO_EVENT_WRITE_END		= 1,
	SOCKET_IO_THREAD_SHUTDOWN		= 2,
	NUM_SOCKET_IO_EVENTS			= 3,
};

class OverlappedStruct
{
public:
	OVERLAPPED overlap;
	SocketIOEvent event;
	volatile long in_use;
	inline OverlappedStruct(SocketIOEvent ev) : event(ev)
	{
		memset(&overlap, 0, sizeof(OVERLAPPED));
		in_use = 0;
	};

	inline OverlappedStruct()
	{
		memset(&overlap, 0, sizeof(OVERLAPPED));
		in_use = 0;
	}

	inline void reset(SocketIOEvent ev)
	{
		memset(&overlap, 0, sizeof(OVERLAPPED));
		event = ev;
	}

	inline void mark()
	{
		long val = InterlockedCompareExchange(&in_use, 1, 0);
		if(val != 0)
			printf("!!!! Network: Detected double use of read/write event! Previous event was %u.\n", event);
	}

	inline void unmark()
	{
		InterlockedExchange(&in_use, 0);
	}
};

#endif

#endif
