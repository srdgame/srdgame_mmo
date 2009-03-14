#ifndef FIELD_H_
#define FIELD_H_

#include "conversion_cast.h"
#include <string>

namespace srdgame
{
class Field
{
public:
	inline void setup(char* val)
	{
		_val = val;
	}

	template<class T>
	T get();

private:
	char* _val;
};
template<class T>
T Field::get()
{
	return conversion_cast<T, char*>(_val);
}

template< >
bool Field::get()
{
	return conversion_cast<int, char*>(_val) > 0;
}

template<>
std::string Field::get()
{
	return std::string(_val);
}

}

#endif
