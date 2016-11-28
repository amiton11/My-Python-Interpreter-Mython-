#include "Void.h"

bool Void::isPrintable() const
{
	return false;
}
std::string Void::toString() const
{
	return "";
}

Type* Void::clone() const
{
	return new Void();
}