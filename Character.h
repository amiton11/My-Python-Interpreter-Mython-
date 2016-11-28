#ifndef CHARACTER_H
#define CHARACTER_H

#include "type.h"
#include "Boolean.h"

class Character : public Type
{
private:
	char _value;
public:
	Character(char value);
	~Character(){}
	char getValue() const;
	void setValue(char value);
	bool isPrintable() const;
	std::string toString() const;
	Type* clone() const;

	operator bool();
	operator std::string();
	operator int();
	operator char();
	operator double();

	Type* operator+(Type*);
	Type* operator-(Type*);
	Type* operator*(Type*);
	Type* operator==(Type*);
	Type* operator!=(Type*);
	Type* operator>(Type*);
	Type* operator<(Type*);
	Type* operator>=(Type*);
	Type* operator<=(Type*);
};

#endif // CHARACTER_H