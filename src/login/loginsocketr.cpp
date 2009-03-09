#include "loginsocketr.h"
#include "log.h"
#include "packetparser.h"
#include "loginworker.h"
#include "typedefs.h"

using namespace srdgame;

LoginInterSocketR::LoginInterSocketR()
	: LoginSocketBase()
{
}

LoginInterSocketR::~LoginInterSocketR()
{

}

void LoginInterSocketR::on_rev()
{
	//lock_rev_buf();
	BufferBase* buf = get_rev_buf();
	size_t size;
	char* data = buf->get_data(size);
	if (size != 0)
	{
		/*char* new_data = new char[size + 1];
		::memset(new_data, 0, size+1);
		::memcpy(new_data, data, size);
		LogSuccess("LoginServer", "Comming Data: %s", new_data);
		delete[] new_data;*/
		size_t index = 0;
		while (size > index)
		{
			Packet p;
			size_t used = PacketParser::get_singleton().from_inter(p, data + index, size - index);
			if (!used)
			{
				if (size - index >= MAX_PACKET_LEN)
				{
					// we should quit here.close connection.
					this->close();
				}
				break;
			}
			LogDebug("LoginInterSocketR", "One packet received");
			index += used;
			_packets.push(p);
			start_worker();
		}
		buf->free(index);

		// Ask for worker.
	}
	else
	{
		buf->free(size);
	}
	//unlock_rev_buf();
}

void LoginInterSocketR::on_send()
{
}

void LoginInterSocketR::on_connect()
{
}

void LoginInterSocketR::on_close()
{
	LogDebug("LoginServer", "Connection with realm server has been dropdown");
}

void LoginInterSocketR::on_handle(Packet* packet)
{
	switch (packet->op)
	{
		case I_PING:
			// Just ping. Both side command.  Normally server will ping client per 5 seconds?
			packet->param.Int++;
			this->send_packet(packet);
			break;
		case I_OFFLINE:
			// Say goodbye, both side command.
			// TODO: What should we do when realm is going to offline.
			break;
		case I_NOTIFY: // Notify others we are going online. Both side action and its param is the client type: Login = 1, world = 2, Realm = 0,
			// TODO: What should we do when realm is going to be online?
			break;
		case IS_GET_NAME: // Ask for name of client.
			{
				Packet p;
				p.op = IC_NAME;
				p.len = sizeof(Packet);
				p.param.Long = 0;
				send_packet(&p);
			}
			break;
		case IC_NAME:// send back the name in one string
			break;
		case IS_GET_STATUS:
			{
				Packet p;
				p.op = IC_STATUS;
				p.len = sizeof(Packet);
				p.param.Long = LS_READY;
				send_packet(&p);
			}
			break;
		case IC_STATUS: // 
			break;
		case IC_POST_STATUS:
			break;// Post the status to server

		case IS_GET_INFO:
			// ask for more detail info, the reply structure is to be defined.
			{
				Packet p;
				p.op = IC_INFO;
				p.len = sizeof(Packet);
				p.param.Long = 0;
				send_packet(&p);
			}
			break;
		case IC_INFO:
			break;
		default:
			LogWarning("LoginServer", "Unknow packet is received from Realm Server, the opcode is : %d", packet->op);
			break;
	}
}
