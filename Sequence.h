#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Type.h"
#include <vector>

class Sequence : public Type
{
public:
	bool isSequence();
	virtual Type* operator[] (std::vector<Type*> index) const = 0;
	virtual void setAtIndex(Type* index, Type* value)=0;
};
#endif // SEQUENCE_H