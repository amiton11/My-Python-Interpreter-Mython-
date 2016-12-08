#ifndef FLOAT_H
#define FLOAT_H

#include "Type.h"
#include "Boolean.h"

class Float : public Type
{
private:
	double _value;
public:
	Float(double value);
	~Float(){}
	double getValue() const;
	void setValue(double value);
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

	static bool isFloat(const std::string);
};

#endif // FLOAT_H