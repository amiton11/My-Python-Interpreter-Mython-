#ifndef STRING_H
#define STRING_H

#include "Sequence.h"
#include "Boolean.h"

class String : public Sequence
{
private:
	std::string _value;
public:
	String(std::string value);
	~String(){}
	std::string getValue() const;
	void setValue(std::string value);

	Type* operator[] (Type* index) const;
	void setAtIndex(Type* index, Type* value);

	bool isPrintable() const;
	std::string toString() const;
	Type* clone() const;

	operator bool();
	operator std::string();

	Type* operator+(Type*);
	Type* operator*(Type*);
	Type* operator==(Type*);
	Type* operator!=(Type*);
	Type* operator>(Type*);
	Type* operator<(Type*);
	Type* operator>=(Type*);
	Type* operator<=(Type*);
};

#endif // STRING_H