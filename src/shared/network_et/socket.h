/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * Socket implementable class.
 *
 */

#ifndef SOCKET_H
#define SOCKET_H

#include "socketdefs.h"
#include <string>
#include <memory>
#include <iostream>

#include "bipbuffer.h"
#include "mutex.h"

using namespace std;

namespace srdgame
{
class Socket
{
public:
	// Constructor. If fd = 0, it will be assigned 
	Socket(SOCKET fd, size_t sendbuffersize, size_t recvbuffersize);
	
	// Destructor.
	virtual ~Socket();

	// Open a connection to another machine.
	bool connect(const char * addr, size_t Port);

	// Disconnect the socket.
	void disconnect();

	// Accept from the already-set fd.
	void accept(sockaddr_in * addr);

/* Implementable methods */

	// Called when data is received.
	virtual void on_read() {}

	// Called when a connection is first successfully established.
	virtual void on_connect() {}

	// Called when the socket is disconnected from the client (either forcibly or by the connection dropping)
	virtual void on_disconnect() {}

/* Sending Operations */

	// Locks sending mutex, adds bytes, unlocks mutex.
	bool send(const char * data, size_t size);

	// Burst system - Locks the sending mutex.
	inline void burst_begin() { w_mutex.lock(); }

	// Burst system - Adds bytes to output buffer.
	bool burst_send(const char * data, size_t size);

	// Burst system - Pushes event to queue - do at the end of write events.
	void burst_push();

	// Burst system - Unlocks the sending mutex.
	inline void burst_end() { w_mutex.unlock(); }

/* Client Operations */

	// get_ the client's ip in numerical form.
	string get_remote_ip();
	inline size_t get_remote_port() { return ntohs(client.sin_port); }
	inline SOCKET get_fd() { return fd; }
	
/* Platform-specific methods */

	void setup_read_event();
	void read_callback(size_t len);
	void write_callback();

	inline bool is_deleted() { return deleted; }
	inline bool is_connected() { return connected; }
	inline sockaddr_in & get_RemoteStruct() { return client; }
	inline BipBuffer& get_read_Buffer() { return r_buf; }
	inline BipBuffer& get_write_Buffer() { return w_buf; }

/* Deletion */
	void Delete();

	inline in_addr get_remote_addr() { return client.sin_addr; }

protected:

	// Called when connection is opened.
	void _on_connect();
  
	SOCKET fd;

	BipBuffer r_buf;
	BipBuffer w_buf;

	Mutex w_mutex;
	Mutex r_mutex;

	// we are connected? stop from posting events.
        bool connected;

        // We are deleted? Stop us from posting events.
        bool deleted;

	sockaddr_in client;

/* Win32 - IOCP Specific Calls */
#ifdef CONFIG_USE_IOCP

public:

	// Set completion port that this socket will be assigned to.
	inline void set_completion_port(HANDLE cp) { completion_port = cp; }
	
	// Atomic wrapper functions for increasing read/write locks
	inline void inc_send_lock() { Interlockedincrement(&w_lock); }
	inline void dec_send_lock() { Interlockeddecrement(&w_lock); }
	inline bool acquire_send_lock()
	{
		if(w_lock)
			return false;
		else
		{
			inc_send_lock();
			return true;
		}
	}
	OverlappedStruct read_event;
	OverlappedStruct write_event;

private:
	// Completion port socket is assigned to
	HANDLE completion_port;
	
	// w_lock, stops multiple write events from being posted.
	volatile long w_lock;
	
	// Assigns the socket to his completion port.
	void assign_to_completion_port();

#endif

/* Linux - EPOLL Specific Calls */
#ifdef CONFIG_USE_EPOLL
public:
	// Posts a epoll event with the specifed arguments.
	void post_event(size_t events);

	// Atomic wrapper functions for increasing read/write locks
	inline void inc_send_lock() { w_lock_mutex.lock(); w_lock++; w_lock_mutex.unlock(); }
	inline void dec_send_lock() { w_lock_mutex.lock(); w_lock--; w_lock_mutex.unlock(); }
	inline bool has_send_lock() { bool res; w_lock_mutex.lock(); res = (w_lock != 0); w_lock_mutex.unlock(); return res; }
	bool acquire_send_lock()
	{
	  bool rv;
	  w_lock_mutex.lock();
	  if(w_lock != 0)
		rv = false;
	  else
	  {
		rv = true;
		w_lock++;
	  }
	  w_lock_mutex.unlock();
	  return rv;
	}

private:
	unsigned int w_lock;
	Mutex w_lock_mutex;
#endif

/* FreeBSD - kqueue specific calls */
#ifdef CONFIG_USE_KQUEUE
public:
	// Posts a epoll event with the specifed arguments.
	void post_event(int events, bool oneshot);
	// Atomic wrapper functions for increasing read/write locks
	inline void inc_send_lock() { w_lock_mutex.lock(); w_lock++; w_lock_mutex.unlock(); }
	inline void dec_send_lock() { w_lock_mutex.lock(); w_lock--; w_lock_mutex.unlock(); }
	inline bool has_send_lock() { bool res; w_lock_mutex.lock(); res = (w_lock != 0); w_lock_mutex.unlock(); return res; }
	bool acquire_send_lock()
	{
		bool rv;
		w_lock_mutex.lock();
		if(w_lock != 0)
			rv = false;
		else
		{
			rv = true;
			w_lock++;
		}
		w_lock_mutex.unlock();
		return rv;
	}

private:
	unsigned int w_lock;
	Mutex w_mutex;
#endif
};

/** connect to a server.
* @param hostname Hostname or IP address to connect to
* @param port Port to connect to
* @return templated type if successful, otherwise null
*/
template<class T>
T* connect_tcp_sokcet(const char * hostname, unsigned short port)
{
	sockaddr_in conn;
	hostent * host;

	/* resolve the peer */
	host = gethostbyname(hostname);

	if(!host)
		return NULL;

	/* copy into our address struct */
	memcpy(&conn.sin_addr, host->h_addr_list[0], sizeof(in_addr));
	conn.sin_family = AF_INET;
	conn.sin_port = ntohs(port);

	T * s = new T(0);
	if(!s->connect(hostname, port))
	{
		s->Delete();
		return 0;
	}
	return s;	
}

/* Socket Garbage Collector */
#define SOCKET_GC_TIMEOUT 15

class SocketGarbageCollector : public Singleton<SocketGarbageCollector>
{
	map<Socket*, time_t> deletionQueue;
	Mutex lock;
public:
	~SocketGarbageCollector()
	{
		map<Socket*, time_t>::iterator i;
		for(i=deletionQueue.begin();i!=deletionQueue.end();++i)
			delete i->first;
	}

	void update()
	{
		map<Socket*, time_t>::iterator i, i2;
		time_t t = UNIXTIME;
		lock.lock();
		for(i = deletionQueue.begin(); i != deletionQueue.end();)
		{
			i2 = i++;
			if(i2->second <= t)
			{
				delete i2->first;
				deletionQueue.erase(i2);
			}
		}
		lock.unlock();
	}

	void queue_socket(Socket * s)
	{
		lock.lock();
		deletionQueue.insert( map<Socket*, time_t>::value_type( s, UNIXTIME + SOCKET_GC_TIMEOUT ) );
		lock.unlock();
	}
};

#define sSocketGarbageCollector SocketGarbageCollector::get_singleton()
}
#endif
