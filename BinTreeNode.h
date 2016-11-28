#ifndef BIN_TREE_NODE_H
#define BIN_TREE_NODE_H

#include "type.h"

class BinTreeNode
{
private:
	BinTreeNode* _leftNode;
	BinTreeNode* _rightNode;
	Type* _value;

public:
	BinTreeNode(Type* value);
	BinTreeNode(Type* value, BinTreeNode* leftNode, BinTreeNode* rightNode);
	BinTreeNode(const BinTreeNode& other);
	~BinTreeNode();

	bool hasLeft() const;
	bool hasRight() const;

	Type* getValue() const;
	void setValue(Type* value);
	BinTreeNode* getLeft() const;
	BinTreeNode* getRight() const;
	void setLeft(BinTreeNode*);
	void setRight(BinTreeNode*);

	Type* calcTree() const;
};

#endif // BIN_TREE_NODE_H