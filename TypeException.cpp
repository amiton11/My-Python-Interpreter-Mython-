#include "TypeException.h"

const char* TypeException::what() const throw()
{
	return "TypeError: unsupported method";
}