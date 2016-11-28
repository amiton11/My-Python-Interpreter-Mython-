#include "String.h"
#include "Helper.h"
#include "Character.h"

String::String(std::string value)
{
	_value = value;
	_myType = ClassType::StringC;
}
std::string String::getValue() const
{
	return _value;
}
void String::setValue(std::string value)
{
	_value = value;
}
bool String::isPrintable() const
{
	return true;
}
std::string String::toString() const
{
	return "'" + _value + "'";
}

Type* String::operator[] (Type* index) const
{
	if (_value == "")
		return nullptr;
	int i = (int)(*index);
	i = i % _value.size();
	if (i < 0)
		i += _value.size();
	return new Character(_value[i]);
}
void String::setAtIndex(Type* index, Type* item)
{
	if (_value == "")
		return;
	int i = (int)(*index);
	i = i % _value.size();
	if (i < 0)
		i += _value.size();
	_value[i] = (char)(*item);
}

Type* String::clone() const
{
	return new String(_value);
}

String::operator bool()
{
	return _value != "";
}
String::operator std::string()
{
	return _value;
}

Type* String::operator+(Type* other)
{
	return new String(_value + (std::string)(*other));
}
Type* String::operator*(Type* other)
{
	std::string newVal = "";
	int multiplyBy = (int)(*other);
	for (int i = 0; i < multiplyBy; i++)
		newVal += _value;
	return new String(newVal);
}
Type* String::operator==(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) == this;
	return (Type*)new Boolean(_value == (std::string)(*other));
}
Type* String::operator!=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) != this;
	return new Boolean(_value != (std::string)(*other));
}
Type* String::operator>(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) < this;
	return new Boolean(_value > (std::string)(*other));
}
Type* String::operator<(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) > this;
	return new Boolean(_value < (std::string)(*other));
}
Type* String::operator>=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) <= this;
	return new Boolean(_value >= (std::string)(*other));
}
Type* String::operator<=(Type* other)
{
	if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) >= this;
	return new Boolean(_value <= (std::string)(*other));
}