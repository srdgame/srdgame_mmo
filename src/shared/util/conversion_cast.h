#ifndef CONVERSION_CAST_H_
#define CONVERSION_CAST_H_

#include <string>
#include <sstream>

namespace srdgame
{

template <typename DT, typename ST>
DT conversion_cast(const ST& val)
{
	std::stringstream ss;
	ss << val;
	DT result;
	ss >> result;
	return result;
}

template <>
inline std::string conversion_cast(const std::string& val)
{
	return val;
}

}
#endif
