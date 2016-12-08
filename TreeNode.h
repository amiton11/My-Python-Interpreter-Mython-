#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "Type.h"
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

	void reverseChild();
};

#endif // TREE_NODE_H