#ifndef NAME_ERROR_EXCEPTION_H
#define NAME_ERROR_EXCEPTION_H

#include "InterperterException.h"
#include <iostream>

class NameErrorException : public InterperterException
{
private:
	std::string _name;
public:
	NameErrorException(std::string name);
	std::string getName() const;
	void setName(std::string name);
	virtual const char* what() const throw();
};

#endif // NAME_ERROR_EXCEPTION_H