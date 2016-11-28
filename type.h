#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include "TypeException.h"

enum ClassType
{
	BooleanC,
	IntegerC,
	CharC,
	FloatC,
	ListC,
	StringC,
	VoidC,
	FunctionC,
	OperandC
};

class Type
{
protected:
	bool _isTemp = false;
	ClassType _myType = ClassType::VoidC;
	bool _shouldClone = true;
public:
	void setIsTemp(bool isTemp);
	bool getIsTemp() const;
	ClassType getTypeName() const;
	bool shouldClone() const;
	virtual bool isPrintable() const = 0;
	virtual std::string toString() const = 0;
	virtual Type* clone() const = 0;

	virtual bool isSequence();

	virtual operator bool();
	virtual operator std::string();
	virtual operator int();
	virtual operator char();
	virtual operator double();

	virtual Type* operator+(Type*);
	virtual Type* operator-(Type*);
	virtual Type* operator*(Type*);
	virtual Type* operator/(Type*);
	virtual Type* operator%(Type*);
	virtual Type* operator==(Type*);
	virtual Type* operator!=(Type*);
	virtual Type* operator>(Type*);
	virtual Type* operator<(Type*);
	virtual Type* operator>=(Type*);
	virtual Type* operator<=(Type*);
	virtual Type* operator&&(Type*);
	virtual Type* operator||(Type*);
};

#endif //TYPE_H
