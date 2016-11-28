#include "Character.h"
#include "String.h"
#include "TypeException.h"

Character::Character(char value)
{
	_value = value;
	_myType = ClassType::CharC;
}

char Character::getValue() const
{
	return _value;
}
void Character::setValue(char value)
{
	_value = value;
}

bool Character::isPrintable() const
{
	return true;
}
std::string Character::toString() const
{
	std::string returnVal = " ";
	returnVal[0] = _value;
	return returnVal;
}

Type* Character::clone() const
{
	return new Character(_value);
}

Character::operator bool()
{
	return _value != 0;
}
Character::operator std::string()
{
	return toString();
}
Character::operator int()
{
	return (int)_value;
}
Character::operator char()
{
	return _value;
}
Character::operator double()
{
	return (double)_value;
}

Type* Character::operator+(Type* other)
{
	switch (other->getTypeName())
	{
	case ClassType::CharC:
	case ClassType::StringC:
		return new String(_value + (std::string)(*other));
		break;
	default:
		int valToAdd = (int)(*other);
		if (valToAdd > 255 || valToAdd < 0)
			throw new TypeException();
		return new Character(_value + valToAdd);
		break;
	}
}
Type* Character::operator-(Type* other)
{
	int valToAdd = (int)(*other);
	if (valToAdd > 255 || valToAdd)
		throw new TypeException();
	return new Character(_value - valToAdd);
}
Type* Character::operator*(Type* other)
{
	String* str = new String(toString());
	return (*str) * other;
}
Type* Character::operator==(Type* other)
{
	if (other->getTypeName() == ClassType::StringC)
		return new Boolean(toString() == (std::string)(*other));
	else if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) == this;
	return new Boolean((int)_value == (int)(*other));
}
Type* Character::operator!=(Type* other)
{
	if (other->getTypeName() == ClassType::StringC)
		return new Boolean(toString() != (std::string)(*other));
	else if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) != this;
	return new Boolean((int)_value != (int)(*other));
}
Type* Character::operator>(Type* other)
{
	if (other->getTypeName() == ClassType::StringC)
		return new Boolean(toString() > (std::string)(*other));
	else if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) < this;
	return new Boolean((int)_value > (int)(*other));
}
Type* Character::operator<(Type* other)
{
	if (other->getTypeName() == ClassType::StringC)
		return new Boolean(toString() < (std::string)(*other));
	else if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) > this;
	return new Boolean((int)_value < (int)(*other));
}
Type* Character::operator>=(Type* other)
{
	if (other->getTypeName() == ClassType::StringC)
		return new Boolean(toString() >= (std::string)(*other));
	else if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) <= this;
	return new Boolean((int)_value >= (int)(*other));
}
Type* Character::operator<=(Type* other)
{
	if (other->getTypeName() == ClassType::StringC)
		return new Boolean(toString() <= (std::string)(*other));
	else if (other->getTypeName() == ClassType::BooleanC)
		return (*(Boolean*)other) >= this;
	return new Boolean((int)_value <= (int)(*other));
}