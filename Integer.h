#ifndef INTEGER_H
#define INTEGER_H

#include "Type.h"
#include "Boolean.h"

class Integer : public Type
{
private:
	int _value;
public:
	Integer(int value);
	~Integer(){}
	int getValue() const;
	void setValue(int value);
	bool isPrintable() const;
	std::string toString() const;
	Type* clone() const;

	operator bool();
	operator std::string();
	operator int();
	operator double();

	Type* operator+(Type*);
	Type* operator-(Type*);
	Type* operator*(Type*);
	Type* operator/(Type*);
	Type* operator%(Type*);
	Type* operator==(Type*);
	Type* operator!=(Type*);
	Type* operator>(Type*);
	Type* operator<(Type*);
	Type* operator>=(Type*);
	Type* operator<=(Type*);
};

#endif // INTEGER_H