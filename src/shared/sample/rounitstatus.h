#ifndef RO_UINT_STATUS_H_
#define RO_UINT_STATUS_H_

namespace ro
{
struct RoUnitStatus
{
	RoUnitStatus() : _option(0), _skill_state(0), _body_state(0), _health_state(0), _speed(0)
	{
	}
	int _option;
	int _skill_state; // opt3
	int _body_state; // opt1
	int _health_state; // opt2
	//int _cout;
	int _speed;
	int _head_dir;

	struct {
		unsigned active : 1;
		unsigned menu_or_input : 1;
		unsigned dead_sit : 1;
	} _state;
};
}

#endif
