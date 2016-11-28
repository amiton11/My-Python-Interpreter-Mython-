#ifndef PARSER_H
#define PARSER_H

#include "InterperterException.h"
#include "IndentationException.h"
#include "SyntaxException.h"
#include "NameErrorException.h"
#include "type.h"
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
#include "Operand.h"
#include "BinTreeNode.h"
#include "Float.h"
#include "Character.h"

class Parser
{
	friend class Function;
public:
	static Type* parseString(std::string str, std::unordered_map<std::string, Type*>* localVarMap = nullptr) throw();
	static Type* getType(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static void freeMemory(std::unordered_map<std::string, Type*>* varMap = nullptr);
	
private:

	static bool isLegalVarName(const std::string& str);
	static bool isLegalFuncCall(const std::string& str);
	static bool isLegalAssigment(const std::string& str);
	static bool isLegalReturn(const std::string& str);
	static bool isLegalComplex(const std::string& str);
	static bool isLegalType(const std::string& str);
	static bool isLegalIndexer(const std::string& str);

	static Type* getVariableValue(const std::string &str, std::unordered_map<std::string, Type*>* varMap = nullptr);
	static Type* getValue(std::string valueStr, std::unordered_map<std::string, Type*>* varMap = nullptr);

	static std::vector<Type*> stringToVector(const std::string &str, std::unordered_map<std::string, Type*>* varMap = nullptr);
	static std::vector<std::string> getKeyWords(const std::string &str, char seperator=',');
	static std::vector<std::string> getComplexWords(const std::string &str);

	/*static Type* getValueAtIndex(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static void setValueAtIndex(const std::string &str, Type* val, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static std::pair<Type*, Type*> getVarAndIndx(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static std::pair<int, int> getBrackets(std::string str);*/

	static void makeAssignment(const std::string& str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static void assignReturnValue(const std::string& str, std::unordered_map<std::string, Type*>* localVarMap);
	static Function* assignFunc(const std::string &str);

	static Type* callFunc(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);

	static std::string getFuncName(const std::string& str);

	static Type* calcComplex(const std::string& str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
	static BinTreeNode* createTree(const std::string& str, std::unordered_map<std::string, Type*>* localVarMap = nullptr);
};

#endif //PARSER_H
