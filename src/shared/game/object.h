#ifndef OBJECT_H_
#define OBJECT_H_


namespace srdgame
{

class Position;
class Object
{
public:
	virtual ~Object(){
	};

public:
	virtual bool move_to(Position* pos, Object* by = NULL){return false;}
	virtual bool clicked(Object* by){return false;}
	virtual bool attacked(Object* by){return false;}

	virtual Position* get_pos() = 0; // implement this by sub classes.
protected:
};

}

#endif
