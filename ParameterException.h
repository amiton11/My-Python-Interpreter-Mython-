#ifndef PARAMETER_ERROR_EXCEPTION_H
#define PARAMETER_ERROR_EXCEPTION_H

#include "InterperterException.h"

class ParameterException : public InterperterException
{
public:
	virtual const char* what() const throw();
};

#endif // PARAMETER_ERROR_EXCEPTION_H