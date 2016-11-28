#ifndef LIST_H
#define LIST_H

#include "Sequence.h"
#include <iostream>
#include <vector>

class List : public Sequence
{
private:
	std::vector<Type*> _value;
public:
	List(std::vector<Type*> value);
	List();
	~List();
	std::vector<Type*> getValue() const;
	void setValue(std::vector<Type*> value);

	Type* operator[] (Type* index) const;
	void setAtIndex(Type* index, Type* value);

	void push(Type*);

	bool isPrintable() const;
	std::string toString() const;
	Type* clone() const;

	Type* operator+(Type*);
	Type* operator*(Type*);

	operator bool();
	operator std::string();
};

#endif // LIST_H