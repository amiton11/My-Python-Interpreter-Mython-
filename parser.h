#ifndef PARSER_H
#define PARSER_H

#include "InterperterException.h"
#include "IndentationException.h"
#include "SyntaxException.h"
#include "NameErrorException.h"
#include "Type.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include "Helper.h"
#include "Boolean.h"
#include "Integer.h"
#include "Sequence.h"
#include "Function.h"
#include "String.h"
#include "Void.h"
#include <vector>
#include "List.h"
#include "TreeNode.h"
#include "Float.h"
#include "Character.h"

class Parser
{
	friend class Function;
public:
	static Type* parseString(std::string str) throw();
	static void freeMemory(std::unordered_map<std::string, Type*>* varMap = nullptr);
	
private:

	static Type* getType(const std::string &str);

	static bool isLegalFuncAssign(const std::string& str);
	static bool isLegalVarName(const std::string& str);
	static bool isLegalType(const std::string& str);
	static bool isLegalReturn(const std::string& str);
	/*static bool isLegalFuncCall(const std::string& str);
	static bool isLegalAssigment(const std::string& str);
	static bool isLegalComplex(const std::string& str);
	static bool isLegalIndexer(const std::string& str);*/

	static Type* getVariableValue(const std::string &str, std::unordered_map<std::string, Type*>* varMap = nullptr);
	
	static TreeNode* getComplexTree(const std::string &str);
	static TreeNode* getInsideTree(const std::string &str, const std::string &val);

	static int findCloser(std::pair<const char, const char> openerAndCloser, const std::string &str, int pos = 0);
	static int findCloser(char opener, char closer, const std::string &str, int pos = 0);
	
	static std::string getCleanStr(std::string &str);

	static std::vector<std::pair<char, std::pair<int, int>>> findInsideRange(const std::string &str);

	static Type* runCommand(TreeNode* commandTree, std::unordered_map<std::string, Type*>* varMap = nullptr);

	static std::vector<Type*> getIndexRange(TreeNode* indexTree, std::unordered_map<std::string, Type*>* varMap = nullptr);
	static void indexRangeRecursive(TreeNode* indexTree, std::vector<Type*>& indexVec, std::unordered_map<std::string, Type*>* varMap = nullptr);

	static Type* getDefaultVal(std::string& str, std::unordered_map<std::string, Type*>* varMap = nullptr);
	
	/*static Type* getValueAtIndex(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static void setValueAtIndex(const std::string &str, Type* val, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static std::pair<Type*, Type*> getVarAndIndx(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static std::pair<int, int> getBrackets(std::string str);*/

	static void makeAssignment(const std::string& varName, Type* assignVal, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static Function* assignFunc(const std::string &str);

	static Void* assignAndReturn(std::string varName, Type* val, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static Void* getTempVoid();
};

#endif //PARSER_H
