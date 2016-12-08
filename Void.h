#ifndef VOID_H
#define VOID_H

#include "Type.h"

class Void : public Type
{
private:
public:
	Void(){}
	~Void(){}
	bool isPrintable() const;
	std::string toString() const;
	Type* clone() const;
};


#endif // VOID_H