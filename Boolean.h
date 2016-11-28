#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "type.h"

class Boolean : public Type
{
private:
	bool _value;
public:
	Boolean(bool value);
	~Boolean(){}
	bool getValue() const;
	void setValue(int value);
	bool isPrintable() const;
	std::string toString() const;
	Type* clone() const;

	operator bool();
	operator std::string();
	operator int();
	operator double();
	operator char();

	Type* operator>(Type*);
	Type* operator<(Type*);
	Type* operator>=(Type*);
	Type* operator<=(Type*);
};

#endif // BOOLEAN_H