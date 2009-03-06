#include "realmsocket.h"
#include "log.h"

using namespace srdgame;

void RealmSocket::on_rev()
{
	lock_rev_buf();
	BufferBase* buf = get_rev_buf();
	size_t size;
	char* data = buf->get_data(size);
	if (size != 0)
	{
		char* new_data = new char[size + 1];
		::memset(new_data, 0, size+1);
		::memcpy(new_data, data, size);
		LogSuccess("RealmServer", "Comming Data: %s", new_data);
		delete[] new_data;
	}
	buf->free(size);
	unlock_rev_buf();
}

void RealmSocket::on_send()
{
}

void RealmSocket::on_connect()
{
}

void RealmSocket::on_close()
{
}
