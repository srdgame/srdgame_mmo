#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <string>
#include <map>

namespace srdgame
{
enum AttributeType
{
};
class Attribute
{
public:
	Attribute();
	virtual ~Attribute();

public:
	long get(AttributeType type)
	{
		return _attrs[type];
	}
	void set(AttributeType type, long val)
	{
		_attrs[type] = val;
	}

private:
	std::map<AttributeType, long> _attrs;
};
}

#endif
