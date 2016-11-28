#ifndef TYPE_EXCEPTION_H
#define TYPE_EXCEPTION_H

#include "InterperterException.h"

class TypeException : public InterperterException
{
public:
	virtual const char* what() const throw();
};


#endif // TYPE_EXCEPTION_H