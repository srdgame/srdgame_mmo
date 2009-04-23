#ifndef PLAYER_H_
#define PLAYER_H_
#include <string>
#include "mutex.h"

#include "ro_defs.h"
#include "rocharinfo.h"

using ro::RoCharInfo;

namespace srdgame
{
class Map;
class WorldSocket;
class Position;
struct Packet;
class RoUnit;
class Player
{
public:
	Player(int acc_id, int id);
	// Will be deleted by WorldMgr, triggered by WorldSocket::on_close();
	virtual ~Player();
	
	virtual void set_info(RoCharInfo* info);
	virtual RoCharInfo* get_info() const; // warry about this.

	virtual std::string get_last_map();
	virtual bool set_map(Map* map);

	virtual int get_id() const;
	virtual int get_acc_id() const;

	virtual void bind(WorldSocket* socket);
	virtual void send_packet(Packet* p);

	virtual void on_handle(Packet* p);

	virtual void load_end_ack();

	virtual RoUnit* get_unit();
	// add item.
	virtual void add_item(ro::RoCharItem& item);
protected:
	void update_look();
	void update_look(ro::RoLookType type, int val);
	void send_friend_list();
	void send_items();

	void equip_item(short index, short pos);
	void unequip_item(short index);
protected:
	Mutex _lock;
	WorldSocket* _socket;
	RoUnit* _unit;
	int _acc_id;
	int _id;
	Map* _map;

	Mutex _info_lock;
	RoCharInfo* _info;
};
}

#endif
