#include "List.h"
#include "Helper.h"

List::List(std::vector<Type*> value) : List()
{
	for (int i = 0; i < value.size(); i++)
	{
		if (value[i]->shouldClone())
			_value.push_back(value[i]->clone());
		else
			_value.push_back(value[i]);
	}
}
List::List()
{
	_myType = ClassType::ListC;
	_shouldClone = false;
}
List::~List()
{
	for each (Type* item in _value)
		delete item;
}
std::vector<Type*> List::getValue() const
{
	return _value;
}
void List::setValue(std::vector<Type*> value)
{
	_value = value;
}
Type* List::operator[] (std::vector<Type*> index) const
{
	std::vector<Type*> newList;
	int start = 0, end = _value.size(), step = 1, i;
	switch (index.size())
	{
	case 1:
		i = getFixedIndex((int)(*index[0]));
		if (_value[i]->shouldClone())
			return _value[i]->clone();
		return _value[i];
		break;
	case 2:
		if (index[0]->getTypeName() != ClassType::VoidC)
			start = getFixedIndex((int)(*index[0]));
		if (index[1]->getTypeName() != ClassType::VoidC)
			end = getFixedIndex((int)(*index[1]));
		return new List(std::vector<Type*>(_value.begin() + start, _value.begin() + end));
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
			end = -1;
		if (step > 0)
		{
			while (start < end)
			{
				if (_value[start]->shouldClone())
					newList.push_back(_value[start]->clone());
				else
					newList.push_back(_value[start]);
				start += step;
			}
		}
		else if (step < 0)
		{
			while (start > end)
			{
				if (_value[start]->shouldClone())
					newList.push_back(_value[start]->clone());
				else
					newList.push_back(_value[start]);
				start += step;
			}
		}
		else
			throw new InterperterException(); // should be a different error
		
		return new List(newList);
		break;
	default:
		throw new InterperterException(); // should be an index exception
		break;
	}
}
void List::setAtIndex(Type* index, Type* item)
{
	int i = getFixedIndex((int)(*index));
	_value[i] = item;
}

int List::getFixedIndex(int index) const
{
	if (index < 0)
		index += _value.size();
	if (index < 0 || index > _value.size() - 1)
		throw new InterperterException(); // should be an index error;
	return index;
}

void List::push(Type* item)
{
	if (item->shouldClone())
		_value.push_back(item->clone());
	else
		_value.push_back(item);
}

bool List::isPrintable() const
{
	return true;
}
std::string List::toString() const
{
	if (_value.empty())
		return "[]";

	std::string desc = _value[0]->toString();
	for (int i = 1; i < _value.size(); i++)
		desc += ", " + _value[i]->toString();

	return '[' + desc + ']';
}
Type* List::clone() const
{
	List* newList = new List();
	for each (Type* item in _value)
		newList->push(item);
	return newList;
}

Type* List::operator+(Type* other)
{
	List* returnList = (List*)clone();
	if (other->getTypeName() != ClassType::ListC)
		returnList->push(other);
	else
		for each (auto item in ((List*)other)->getValue())
			returnList->push(item);
	return returnList;
}
Type* List::operator*(Type* other)
{
	int num = (int)(*other);
	List* returnList = new List();
	for (int i = 0; i < num; i++)
		for (auto x: _value)
			returnList->push(x);
	return returnList;
}

List::operator bool()
{
	return !_value.empty();
}
List::operator std::string()
{
	return toString();
}