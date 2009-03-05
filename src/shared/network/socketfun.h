#ifndef SOCKET_FUNC_H_
#define SOCKET_FUNC_H_

namespace srdgame
{
class SocketFunc
{
public:
	inline static SOCKET create_tcp_socket()
	{
		return  ::socket(AF_INET, SOCK_STREAM, 0);
	}
	inline static SOCKET create_udp_socket();

	inline static bool blocking(SOCKET fd)
	{
		unsigned int arg = 0;
        	return (::ioctl(fd, FIONBIO, &arg) == 0);
	}
	inline static bool nonblocking(SOCKET fd)
	{
		unsigned int arg = 1;
	        return (::ioctl(fd, FIONBIO, &arg) == 0);
	}

	inline static bool disable_buffering(SOCKET fd)
	{
		unsigned int arg = 1;
        	return (::setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
	}
	inline static bool enable_buffering(SOCKET fd)
	{
        	unsigned int arg = 0;
	        return (::setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0); 
	}

	inline static void close_socket(SOCKET fd)
	{
        	::shutdown(fd, SHUT_RDWR);
        	::close(fd);
	}

	inline static void reuse_addr(SOCKET fd)
	{
		unsigned int option = 1;
	        ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);
	}
	inline static bool set_timeout(SOCKET fd, unsigned int time)
	{
		struct timeval to;
		to.tv_sec = time;
		to.tv_usec = 0;
		if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&to, (socklen_t)sizeof(to)) != 0) return false;
		return (::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, (socklen_t)sizeof(to)) == 0);
	}


    	inline static bool set_send_buf_size(SOCKET fd, unsigned int size)
    	{
        	return (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size)) == 0);
    	}

   	inline static bool set_recv_buf_size(SOCKET fd, unsigned int size)
    	{
        	return (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size)) == 0);
    	}
};
}

#endif
