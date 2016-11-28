#include "Boolean.h"

Boolean::Boolean(bool value)
{
	_value = value;
	_myType = ClassType::BooleanC;
}
bool Boolean::getValue() const
{
	return _value;
}
void Boolean::setValue(int value)
{
	_value = value;
}
bool Boolean::isPrintable() const
{
	return true;
}
std::string Boolean::toString() const
{
	if (_value)
		return "True";
	return "False";
}
Type* Boolean::clone() const
{
	return new Boolean(_value);
}

Boolean::operator bool()
{
	return _value;
}
Boolean::operator std::string()
{
	return toString();
}
Boolean::operator int()
{
	return _value ? 1 : 0;
}
Boolean::operator double()
{
	return _value ? 1.0 : 0.0;
}
Boolean::operator char()
{
	return _value ? 't' : 'f';
}

Type* Boolean::operator>(Type* other)
{
	return new Boolean(_value == true && (bool)(*other) == false);
}
Type* Boolean::operator<(Type* other)
{
	return new Boolean(_value == false && (bool)(*other) == true);
}
Type* Boolean::operator>=(Type* other)
{
	return new Boolean(_value == true || (bool)(*other) == false);
}
Type* Boolean::operator<=(Type* other)
{
	return new Boolean(_value == false || (bool)(*other) == true);
}