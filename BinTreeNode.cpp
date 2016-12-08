#include "BinTreeNode.h"
#include "Operand.h"

TreeNode::TreeNode(std::string value) : _value(value) {}
TreeNode::TreeNode(std::string value, TreeNode* onlyChild) : _value(value)
{
	_childs.push_back(onlyChild);
}
TreeNode::TreeNode(std::string value, TreeNode* left, TreeNode* right) : _value(value)
{
	_childs.push_back(left);
	_childs.push_back(right);
}
TreeNode::TreeNode(std::string value, std::vector<TreeNode*> childs) : _value(value), _childs(childs) {}
TreeNode::TreeNode(const TreeNode& other) : TreeNode(other._value, other._childs){}

TreeNode::~TreeNode()
{
	for each (auto child in _childs)
		if (child != nullptr)
			delete child;
}

bool TreeNode::hasChilds() const
{
	return !_childs.empty();
}
int TreeNode::childCount() const
{
	return _childs.size();
}

std::string TreeNode::getValue() const
{
	return _value; 
}
void TreeNode::setValue(std::string value)
{
	_value = value;
}

std::vector<TreeNode*> TreeNode::getChilds() const
{
	return _childs;
}
TreeNode* TreeNode::getChildAt(int idx) const
{
	return _childs[idx];
}

void TreeNode::setChilds(std::vector<TreeNode*> childs)
{
	_childs = childs;
}
void TreeNode::setChildAt(TreeNode* value, int idx)
{
	_childs[idx] = value;
}

void TreeNode::pushChild(TreeNode* child)
{
	_childs.push_back(child);
}


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