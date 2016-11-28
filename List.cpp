#include "List.h"
#include "Helper.h"

List::List(std::vector<Type*> value) : List()
{
	_value = value;
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
Type* List::operator[] (Type* index) const
{
	if (_value.empty())
		return nullptr;
	int i = (int)(*index);
	i = i % _value.size();
	if (i < 0)
		i += _value.size();
	return _value[i];
}
void List::setAtIndex(Type* index, Type* item)
{
	if (_value.empty())
		return;
	int i = (int)(*index);
	i = i % _value.size();
	if (i < 0)
		i += _value.size();
	_value[i] = item;
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
	returnList->push(other);
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