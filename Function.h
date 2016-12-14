#ifndef FUNCTION_H
#define FUNCTION_H

#include "Type.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "ParameterException.h"
#include "parser.h"
#include "TreeNode.h"

class Function : public Type
{
private:
	std::vector<TreeNode*> _lines;
	std::vector<std::string> _parametersNames;
	std::unordered_map<std::string, Type*> _localVars;

public:
	Function(std::vector<std::string> parametersNames);
	Function(std::vector<std::string> parametersNames, std::vector<TreeNode*> lines);
	~Function(){};

	std::vector<TreeNode*> getLines() const;
	void setLines(std::vector<TreeNode*> lines);
	std::vector<std::string> getParameters() const;
	void setParameters(std::vector<std::string> parameters);

	bool parseInto(std::string preIndent = "");

	Type* run(std::vector<Type*> parameters);

	void freeLocals();

	bool isPrintable() const { return false; }
	std::string toString() const { return ""; }
	Type* clone() const;
};

#endif // FUNCTION_H