#ifndef PLAYER_H_
#define PLAYER_H_
#include <string>
#include "mutex.h"

//#include "charinfo.h"

namespace srdgame
{
struct CharInfo;
class Map;
class WorldSocket;
class Position;
struct Packet;
class Player
{
public:
	Player(int acc_id, int id);
	virtual ~Player();
	
	virtual void set_info(CharInfo* info);
	virtual CharInfo* get_info(); // warry about this.

	virtual std::string get_last_map();
	virtual bool set_map(Map* map);

	virtual int get_id();
	virtual int get_acc_id();

	virtual void bind(WorldSocket* socket);
	virtual void send_packet(Packet* p);

	virtual void on_handle(Packet* p);

protected:
	void update_look();
	void send_friend_list();

protected:
	Mutex _lock;
	WorldSocket* _socket;
	int _acc_id;
	int _id;
	CharInfo* _info;
	Map* _map;
};
}

#endif
