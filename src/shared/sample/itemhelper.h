#ifndef ITEM_HELPER_H_
#define ITEM_HELPER_H_

#include "itemdefs.h"
#include "roitemdb.h"

namespace ro
{
inline bool isequip2(const RoDBItem* data)
{ 
	switch(data->_item_type) {
		case IT_WEAPON:
		case IT_ARMOR:
		case IT_AMMO:
			return true;
		default:
			return false;
	}
}
inline bool isstackable2(const RoDBItem* item)
{
  switch(item->_item_type) {
	  case IT_WEAPON:
	  case IT_ARMOR:
	  case IT_PETEGG:
	  case IT_PETARMOR:
		  return false;
	  default:
		  return true;
  }
}
inline int equippoint(const RoDBItem* item)
{
	int ep = 0;

	if (!isequip2(item))
		return 0; //Not equippable by players.
	
	ep = item->_equip_point;
	// TODO:
	/*if(db->_look == W_DAGGER	||
		db->_look == W_1HSWORD ||
		db->_look == W_1HAXE)
	{
		if(ep == EQP_HAND_R) && (pc_checkskill(sd,AS_LEFT) > 0 || (sd->class_&MAPID_UPPERMASK) == MAPID_ASSASSIN))
			return EQP_ARMS;
	}*/
	return ep;
}

}

#endif
