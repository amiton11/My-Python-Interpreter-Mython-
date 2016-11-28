#include "ParameterException.h"

const char* ParameterException::what() const throw()
{
	return "ParameterError: illegal call of function";
}