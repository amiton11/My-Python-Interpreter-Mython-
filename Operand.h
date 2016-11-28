#ifndef OPERAND_H
#define OPERAND_H

#include "type.h"

enum OperandType
{
	Add = '+',
	Sub = '-',
	Mul = '*',
	Div = '/',
	Mod = '%',
	Equ = '=' * '=' % 256,
	Big = '>',
	Sml = '<',
	NotEqu = '!' * '=' % 256,
	BigEqu = '>' * '=' % 256,
	SmlEqu = '<' * '=' % 256,
	Or = '|' * '|' % 256,
	And = '&' * '&' % 256,
	Asgn = '=',
	AsgnMul = '*' * '=' % 256,
	AsgnDiv = '/' * '=' % 256,
	AsgnAdd = '+' * '=' % 256,
	AsgnSub = '-' * '=' % 256
};

class Operand : public Type
{
private:
	OperandType _opType;
public:
	Operand(OperandType opType);
	~Operand(){}
	OperandType getOpType() const;
	void setOpType(OperandType opType);
	bool isPrintable() const;
	std::string toString() const;
	Type* clone() const;
};

#endif // OPERAND_H