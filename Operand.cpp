#include "Operand.h"

Operand::Operand(OperandType opType) : _opType(opType)
{
	_myType = ClassType::OperandC;
} 

OperandType Operand::getOpType() const
{
	return _opType;
}
void Operand::setOpType(OperandType opType)
{
	_opType = opType;
}

bool Operand::isPrintable() const
{
	return true;
}
std::string Operand::toString() const
{
	return "" + (char)_opType;
}
Type* Operand::clone() const
{
	return new Operand(_opType);
}