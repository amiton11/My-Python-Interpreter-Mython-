#include "Float.h"
#include "Helper.h"
#include <string>

Float::Float(double value)
{
	_value = value;
	_myType = ClassType::FloatC;
}

double Float::getValue() const
{
	return _value;
}
void Float::setValue(double value)
{
	_value = value;
}

bool Float::isPrintable() const
{
	return true;
}
std::string Float::toString() const
{
	return std::to_string(_value);
}

Type* Float::clone() const
{
	return new Float(_value);
}

Float::operator bool()
{
	return _value != 0;
}
Float::operator std::string()
{
	return toString();
}
Float::operator int()
{
	return (int)_value;
}
Float::operator double()
{
	return _value;
}

Type* Float::operator+(Type* other)
{
	return new Float(_value + (double)(*other));
}
Type* Float::operator-(Type* other)
{
	return new Float(_value - (double)(*other));
}
Type* Float::operator*(Type* other)
{
	return new Float(_value * (double)(*other));
}
Type* Float::operator/(Type* other)
{
	return new Float(_value / (double)(*other));
}
Type* Float::operator%(Type* other)
{
	return new Float(fmod(_value, (double)(*other)));
}
Type* Float::operator==(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) == this;
	return new Boolean(_value == (double)(*other));
}
Type* Float::operator!=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) != this;
	return new Boolean(_value != (double)(*other));
}
Type* Float::operator>(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) < this;
	return new Boolean(_value > (double)(*other));
}
Type* Float::operator<(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) > this;
	return new Boolean(_value < (double)(*other));
}
Type* Float::operator>=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) <= this;
	return new Boolean(_value >= (double)(*other));
}
Type* Float::operator<=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) >= this;
	return new Boolean(_value <= (double)(*other));
}

bool Float::isFloat(const std::string str)
{
	char pointIdx = str.find_first_of('.');
	if (pointIdx == std::string::npos)
		return false;
	for (char digit : str.substr(0, pointIdx))
		if (!Helper::isDigit(digit))
			return false;
	for (char digit : str.substr(pointIdx + 1))
		if (!Helper::isDigit(digit))
			return false;
	return true;
}