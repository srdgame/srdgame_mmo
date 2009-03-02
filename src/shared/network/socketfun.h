#ifndef SOCKET_FUNC_H_
#define SOCKET_FUNC_H_

namespace srdgame
{
class SocketFunc
{
public:
	static SOCKET create_tcp_socket();
	static SOCKET create_udp_socket();

	static void blocking(SOCKET fd);
	static void nonblocking(SOCKET fd);

	static void disable_buffering(SOCKET fd);
	static void enable_buffering(SOCKET fd);

	static void close_socket(SOCKET fd);
};
}

#endif
