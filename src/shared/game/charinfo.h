#ifndef CHAR_INFO_H_
#define CHAR_INFO_H_

namespace srdgame
{
class CharInfo
{
public:
	CharInfo();
	virtual ~CharInfo();

public:
	Attribute* get_attr(AttributeType type);
}
}

#endif
