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
inline T Field::get()
{
	return conversion_cast<T, char*>(_val);
}

template< >
inline bool Field::get()
{
	return conversion_cast<int, char*>(_val) > 0;
}

template<>
inline std::string Field::get()
{
	return std::string(_val);
}
template<>
inline char* Field::get()
{
	return _val;
}
template<>
inline char Field::get()
{
	return _val[0];
}

}

#endif
