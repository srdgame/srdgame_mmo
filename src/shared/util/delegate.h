#ifndef DELEGATE_H_
#define DELEGATE_H_

#include <cstddef>

namespace srdgame
{
template <class RT, class PT>
class Delegate
{
public:
	Delegate() : _obj(NULL), _ptr(NULL)
	{
	}
	template < class T, RT (T::*method_ptr)(PT) >
	static Delegate from_method(T* obj)
	{
		Delegate< RT, PT > d;
		d._obj = obj;
		d._ptr = &function_ptr< T, method_ptr >;

		return d;
	}

	RT operator()(PT p) const
	{
		return (*_ptr)(_obj, p);
	}
	operator bool () const
	{
		return _ptr;
	}
	bool operator!() const
	{
		return !(operator bool());
	}

private:
	typedef RT (*FunctionPtr)(void* obj, PT);

	void* _obj;
	FunctionPtr _ptr;

	template < class T, RT (T::*method_ptr)(PT) >
	static RT function_ptr(void* obj, PT p)
	{
		T* pT = static_cast< T* >(obj);
		return (pT->*method_ptr)(p);
	}
};
}
// Delegate d = Delegate<int, int>::from_method<ExampleClass, &ExampleClass::someMethod)(&obj);
#endif
