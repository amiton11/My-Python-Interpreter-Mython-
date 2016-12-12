#include "Integer.h"
#include "Float.h"
#include <string> 

Integer::Integer(int value)
{
	_value = value;
	_myType = ClassType::IntegerC;
}

int Integer::getValue() const
{
	return _value;
}
void Integer::setValue(int value)
{
	_value = value;
}

bool Integer::isPrintable() const
{
	return true;
}
std::string Integer::toString() const
{
	return std::to_string(_value);
}

Type* Integer::clone() const
{
	return new Integer(_value);
}

Integer::operator bool()
{
	return _value != 0;
}
Integer::operator std::string()
{
	return toString();
}
Integer::operator int()
{
	return _value;
}
Integer::operator double()
{
	return (double)_value;
}

Type* Integer::operator+(Type* other)
{
	if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value + (double)(*other));
	else
		return new Integer(_value + (int)(*other));
}
Type* Integer::operator-(Type* other)
{
	if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value - (double)(*other));
	else
		return new Integer(_value - (int)(*other));
}
Type* Integer::operator*(Type* other)
{
	if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value * (double)(*other));
	else
		return new Integer(_value * (int)(*other));
}
Type* Integer::operator/(Type* other)
{
	if (other->getTypeName() == ClassType::FloatC)
	{
		if ((double)(*other) == 0.0)
			throw new InterperterException(); // should be a division by zero exception
		return new Float((double)_value / (double)(*other));
	}
	else
	{
		if ((int)(*other) == 0)
			throw new InterperterException(); // should be a division by zero exception
		return new Integer(_value / (int)(*other));
	}
}
Type* Integer::operator%(Type* other)
{
	if (other->getTypeName() == ClassType::FloatC)
	{
		if ((double)(*other) == 0.0)
			throw new InterperterException(); // should be a division by zero exception
		return new Float(fmod((double)_value, (double)(*other)));
	}
	else
	{
		if ((int)(*other) == 0)
			throw new InterperterException(); // should be a division by zero exception
		return new Integer(_value / (int)(*other));
	}
}
Type* Integer::operator==(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) == this;
	else if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value == (double)(*other));
	else
		return new Integer(_value == (int)(*other));
}
Type* Integer::operator!=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) != this;
	else if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value != (double)(*other));
	else
		return new Integer(_value != (int)(*other));
}
Type* Integer::operator>(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) < this;
	else if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value > (double)(*other));
	else
		return new Integer(_value > (int)(*other));
}
Type* Integer::operator<(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) > this;
	else if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value < (double)(*other));
	else
		return new Integer(_value < (int)(*other));
}
Type* Integer::operator>=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) <= this;
	else if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value >= (double)(*other));
	else
		return new Integer(_value >= (int)(*other));
}
Type* Integer::operator<=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) >= this;
	else if (other->getTypeName() == ClassType::FloatC)
		return new Float((double)_value <= (double)(*other));
	else
		return new Integer(_value <= (int)(*other));
}