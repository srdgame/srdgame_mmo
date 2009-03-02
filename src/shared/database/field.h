#ifndef FIELD_H_
#define FIELD_H_

#include "conversion_cast.h"
#include <string>

namespace srdgame
{
class Field
{
public:
	template<class T>
	inline void set(const T& val)
	{
		_val = conversion_cast<std::string, T>(val);
	}

	template<class T>
	inline T get()
	{
		return conversion_cast<T, std::string>(_val);
	}
private:
	std::string _val;
}
}

#endif
