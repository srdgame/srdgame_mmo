#ifndef OBJECT_H_
#define OBJECT_H_


namespace srdgame
{
class Position;
class Object
{
public:
	~Object(){
	};

public:
	bool move_to(Position* pos){return false;}
	bool clicked(Object* by){return false;}
	bool attacked(Object* by){return false;}

	Position* get_pos() = 0; // implement this by sub classes.
protected:
}
}

#endif
