#ifndef FAMILY_H_
#define FAMILY_H_

struct NpcFamilyInfo
{
	NpcFamilyInfo() : _partner_id(0), _father_id(0), _mother_id(0), _child_id(0)
	{
	}
	int _partner_id;
	int _father_id;
	int _mother_id;
	int _child_id;

};

#endif
