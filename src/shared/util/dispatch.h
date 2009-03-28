#ifndef DISPATCH_H_
#define DISPATCH_H_

#include "delegate.h"
#include <cassert>

namespace srdgame
{
template <class RT, class PT>
class Dispatch
{
	Delegate<RT, PT>* _handlers;
public:
	Dispatch(unsigned int max_index)
	{
		_handlers = new Delegate<RT, PT>[max_index];
	}
	virtual ~Dispatch()
	{
		delete[] _handlers;
	}
	inline void reg(unsigned int index, Delegate<RT, PT>& d)
	{
		*(_handlers + index) = d;
	}
	inline RT operator()(unsigned int index, PT p)
	{
		return call(index, p);
	}
	inline RT call(unsigned int index, PT p)
	{
		Delegate<RT, PT>& d = *(_handlers + index);
		if (d)
		{
			return d(p);
		}
		else
		{
			return default_(p);
		}
	}

protected:
	inline RT default_(PT p)
	{
		assert(false);
		return RT();
	}
};
}

#endif
