#ifndef BIN_TREE_NODE_H
#define BIN_TREE_NODE_H

#include "type.h"
#include <vector>

class TreeNode
{
private:
	std::vector<TreeNode*> _childs;
	std::string _value;

public:
	TreeNode(std::string value);
	TreeNode(std::string value, TreeNode* onlyChild);
	TreeNode(std::string value, TreeNode* left, TreeNode* right);
	TreeNode(std::string value, std::vector<TreeNode*> childs);
	TreeNode(const TreeNode& other);
	~TreeNode();

	bool hasChilds() const;
	int childCount() const;

	std::string getValue() const;
	void setValue(std::string value);

	std::vector<TreeNode*> getChilds() const;
	TreeNode* getChildAt(int idx) const;

	void setChilds(std::vector<TreeNode*> childs);
	void setChildAt(TreeNode* value, int idx);

	void pushChild(TreeNode* child);
};

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