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

Type* String::operator[] (std::vector<Type*> index) const
{
	std::string newStr = "";
	int start = 0, end = _value.size(), step = 1, i;
	switch (index.size())
	{
	case 1:
		i = getFixedIndex((int)(*index[0]));
		return new Character(_value[i]);
		break;
	case 2:
		if (index[0]->getTypeName() != ClassType::VoidC)
			start = getFixedIndex((int)(*index[0]));
		if (index[1]->getTypeName() != ClassType::VoidC)
			end = getFixedIndex((int)(*index[1]));
		return new String(_value.substr(start, end - start));
		break;
	case 3:
		if (index[2]->getTypeName() != ClassType::VoidC)
			step = (int)(*index[2]);
		if (index[0]->getTypeName() != ClassType::VoidC)
			start = getFixedIndex((int)(*index[0]));
		else if (step < 0)
			start = _value.size() - 1;
		if (index[1]->getTypeName() != ClassType::VoidC)
			end = getFixedIndex((int)(*index[1]));
		else if (step < 0)
			end = - 1;
		if (step > 0)
		{
			while (start < end)
			{
				newStr += _value[start];
				start += step;
			}
		}
		else if (step < 0)
		{
			while (start > end)
			{
				newStr += _value[start];
				start += step;
			}
		}
		else
			throw new InterperterException(); // should be a different error

		return new String(newStr);
		break;
	default:
		throw new InterperterException(); // should be an index exception
		break;
	}
}
void String::setAtIndex(Type* index, Type* item)
{
	int i = getFixedIndex((int)(*index));
	_value[i] = (char)(*item);
}

int String::getFixedIndex(int index) const
{
	if (index < 0)
		index += _value.size();
	if (index < 0 || index > _value.size() - 1)
		throw new InterperterException(); // should be an index error;
	return index;
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