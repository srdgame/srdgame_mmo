#ifndef OBJECT_H_
#define OBJECT_H_


namespace srdgame
{
class Position;
class Object
{
public:
	~Object(){
		if (_pos)
			delete _pos;
	};

public:
	bool move_to(Position* pos){return false;}
	bool clicked(Object* by){return false;}
	bool attacked(Object* by){return false;}

	Position* get_pos(){ return _pos;}
protected:
	Position* _pos;
}
}

#endif
