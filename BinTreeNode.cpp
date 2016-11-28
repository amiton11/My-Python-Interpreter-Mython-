#include "BinTreeNode.h"
#include "Operand.h"

BinTreeNode::BinTreeNode(Type* value) : BinTreeNode(value, nullptr, nullptr){}
BinTreeNode::BinTreeNode(Type* value, BinTreeNode* leftNode, BinTreeNode* rightNode)
{
	setValue(value);

	_leftNode = leftNode;
	_rightNode = rightNode;
}
BinTreeNode::BinTreeNode(const BinTreeNode& other) : BinTreeNode(other._value)
{
	if (other.hasLeft())
		_leftNode = new BinTreeNode(*other._leftNode);
	if (other.hasRight())
		_rightNode = new BinTreeNode(*other._rightNode);
}

BinTreeNode::~BinTreeNode()
{
	if (hasLeft())
		delete _leftNode;
	if (hasRight())
		delete _rightNode;
	if (_value->getIsTemp())
		delete _value;
}

bool BinTreeNode::hasLeft() const
{
	return _leftNode != nullptr;
}
bool BinTreeNode::hasRight() const
{
	return _rightNode != nullptr;
}

Type* BinTreeNode::getValue() const
{
	if (_value->shouldClone())
		return _value->clone();
	else
		return _value;
}
void BinTreeNode::setValue(Type* value)
{
	if (value->shouldClone())
	{
		_value = value->clone();
		_value->setIsTemp(true);
	}
	else
		_value = value;
}

BinTreeNode* BinTreeNode::getLeft() const
{
	return _leftNode;
}
BinTreeNode* BinTreeNode::getRight() const
{
	return _rightNode;
}
void BinTreeNode::setLeft(BinTreeNode* binNode)
{
	_leftNode = binNode;
}
void BinTreeNode::setRight(BinTreeNode* binNode)
{
	_rightNode = binNode;
}

Type* BinTreeNode::calcTree() const
{
	if (!hasLeft() || !hasRight())
		return getValue();
	if (_value->getTypeName() != ClassType::OperandC)
		return nullptr;
	Operand* myOp = (Operand*)_value;
	Type* left = _leftNode->calcTree();
	Type* right = _rightNode->calcTree();

	Type* rtrnVal = nullptr;
	switch (myOp->getOpType())
	{
	case OperandType::Add:
		rtrnVal = (*left) + right;
		break;
	case OperandType::Sub:
		rtrnVal = (*left) - right;
		break;
	case OperandType::Mul:
		rtrnVal = (*left) * right;
		break;
	case OperandType::Div:
		rtrnVal = (*left) / right;
		break;
	case OperandType::Mod:
		rtrnVal = (*left) % right;
		break;
	case OperandType::Equ:
		rtrnVal = (*left) == right;
		break;
	case OperandType::NotEqu:
		rtrnVal = (*left) != right;
		break;
	case OperandType::Big:
		rtrnVal = (*left) > right;
		break;
	case OperandType::Sml:
		rtrnVal = (*left) < right;
		break;
	case OperandType::BigEqu:
		rtrnVal = (*left) >= right;
		break;
	case OperandType::SmlEqu:
		rtrnVal = (*left) <= right;
		break;
	case OperandType::And:
		rtrnVal = (*left) && right;
		break;
	case OperandType::Or:
		rtrnVal = (*left) || right;
		break;
	}
	delete left;
	delete right;
	return rtrnVal;
}