#include "type.h"
#include "Boolean.h"

void Type::setIsTemp(bool isTemp)
{
	_isTemp = isTemp;
}
bool Type::getIsTemp() const
{
	return _isTemp;
}
ClassType Type::getTypeName() const
{
	return _myType;
}
bool Type::shouldClone() const
{
	return _shouldClone;
}

bool Type::isSequence() 
{ 
	return false; 
}

Type::operator bool()
{
	throw new TypeException();
}
Type::operator std::string()
{
	throw new TypeException();
}
Type::operator int()
{
	throw new TypeException();
}
Type::operator char()
{
	throw new TypeException();
}
Type::operator double()
{
	throw new TypeException();
}

Type* Type::operator+(Type* other)
{
	throw new TypeException();
}
Type* Type::operator-(Type* other)
{
	throw new TypeException();
}
Type* Type::operator*(Type* other)
{
	throw new TypeException();
}
Type* Type::operator/(Type* other)
{
	throw new TypeException();
}
Type* Type::operator%(Type* other)
{
	throw new TypeException();
}
Type* Type::operator==(Type* other)
{
	return new Boolean((bool)(*this) == (bool)(*other));
}
Type* Type::operator!=(Type* other)
{
	return new Boolean((bool)(*this) != (bool)(*other));
}
Type* Type::operator>(Type* other)
{
	throw new TypeException();
}
Type* Type::operator<(Type* other)
{
	throw new TypeException();
}
Type* Type::operator>=(Type* other)
{
	throw new TypeException();
}
Type* Type::operator<=(Type* other)
{
	throw new TypeException();
}
Type* Type::operator&&(Type* other)
{
	return (Type*)new Boolean((bool)(*this) && (bool)(*other));
}
Type* Type::operator||(Type* other)
{
	return (Type*)new Boolean((bool)(*this) || (bool)(*other));
}