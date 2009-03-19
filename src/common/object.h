#ifndef OBJECT_H_
#define OBJECT_H_

namespace srdgame
{
class Position;
class Object
{
public:
	Object();
	~Object();

public:
	bool move_to(Position* pos){return false;}
	bool clicked(Object* by){return false;}
	bool attacked(Object* by){return false;}
}
}

#endif
