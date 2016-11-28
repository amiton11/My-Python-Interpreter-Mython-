#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "type.h"

class Sequence : public Type
{
public:
	bool isSequence();
	virtual Type* operator[] (Type* index) const = 0;
	virtual void setAtIndex(Type* index, Type* value)=0;
};
#endif // SEQUENCE_H