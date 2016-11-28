#include "NameErrorException.h"

std::string _desc;
NameErrorException::NameErrorException(std::string name)
{
	_name = name;
}
std::string NameErrorException::getName() const
{
	return _name;
}
void NameErrorException::setName(std::string name)
{
	_name = name;
}
const char* NameErrorException::what() const throw()
{
	_desc = "NameError: name '" + _name + "' is not defined";
	return _desc.c_str();
}