#include "TreeNode.h"
#include <iostream>
#include <algorithm>
#include <string>

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
	if (idx > _childs.size() - 1)
		throw new InterperterException();
	return _childs[idx];
}

void TreeNode::setChilds(std::vector<TreeNode*> childs)
{
	_childs = childs;
}
void TreeNode::setChildAt(TreeNode* value, int idx)
{
	if (idx > _childs.size() - 1)
		throw new InterperterException();
	_childs[idx] = value;
}

void TreeNode::pushChild(TreeNode* child)
{
	_childs.push_back(child);
}

void TreeNode::reverseChild()
{
	std::reverse(_childs.begin(), _childs.end());
}

void TreeNode::printTree()
{
	int i;
	if (childCount() == 0)
	{
		if (_value == "")
			std::cout << "Void";
		else
			std::cout << _value;
	}
	else if (_value == "[]" || _value == "{}")
	{
		std::cout << _value[0];
		for (i = 0; i < childCount() - 1; i++)
		{
			if (_childs[i] == nullptr)
				std::cout << "Void";
			else
				_childs[i]->printTree();
			std::cout << ", ";
		}
		if (_childs[i] == nullptr)
			std::cout << "Void";
		else
			_childs[i]->printTree();
		std::cout << _value[1];
	}
	else if (childCount() == 1)
	{
		std::cout << _value << " ";
		if (_childs[0] == nullptr)
			std::cout << "Void";
		else
			_childs[0]->printTree();
	}
	else if (childCount() == 2)
	{
		if (_childs[0] == nullptr)
			std::cout << "Void";
		else
			_childs[0]->printTree();
		if (_value == "()" || _value == "[]*")
		{
			std::cout << _value[0];
			if (_childs[1] == nullptr)
				std::cout << "Void";
			else
				_childs[1]->printTree();
			std::cout << _value[1];
		}
		else
		{
			std::cout << _value;
			if (_childs[1] == nullptr)
				std::cout << "Void";
			else
				_childs[1]->printTree();
		}
	}
}