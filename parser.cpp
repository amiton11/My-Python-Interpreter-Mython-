#include "parser.h"
#include <algorithm>

std::unordered_map<std::string, Type*> _variables;
const std::string reservedWords[] = { "return", "def", "True", "False" };
const std::string sizeTwoOps[] = { "==", "!=", ">=", "<=", "||", "&&" };
const char sizeOneOps[] = { '+', '-', '*', '/', '%', '>', '<' };
const char legalComplex[] = {'[', ']', '(', ')', ','};

Type* Parser::parseString(std::string str, std::unordered_map<std::string, Type*>* localVarMap) throw()
{
	if (str[0] == ' ' || str[0] == '\t')
		throw new IndentationException();
	Helper::trim(str);
	if (str.length() > 0)
	{
		Type* returnVal;
		if (isLegalAssigment(str))
		{
			makeAssignment(str, localVarMap);
			returnVal = new Void();
			returnVal->setIsTemp(true);
			return returnVal;
		}

		if (isLegalFuncCall(str))
		{
			returnVal = callFunc(str, localVarMap);
			if (returnVal == nullptr)
				throw new NameErrorException(str);
			return returnVal;
		}
		returnVal = getType(str);
	 	if (returnVal != nullptr)
		{
			returnVal->setIsTemp(true);
			return returnVal;
		}
		if (isLegalVarName(str))
		{
			returnVal = getVariableValue(str, localVarMap);
			if (returnVal == nullptr)
				throw new NameErrorException(str);
			return returnVal;
		}
		if (isLegalIndexer(str))
		{
		}
		if (isLegalComplex(str))
		{
			returnVal = calcComplex(str, localVarMap);
			returnVal->setIsTemp(true);
			return returnVal;
		}
		if (localVarMap != nullptr && isLegalReturn(str))
		{
			assignReturnValue(str, localVarMap);
			returnVal = new Void();
			returnVal->setIsTemp(true);
			return returnVal;
		}
		Function* newFunc = assignFunc(str);
		if (newFunc != nullptr)
		{
			bool parseSuccesful = newFunc->parseInto();
			if (!parseSuccesful)
				throw new SyntaxException();
			returnVal = new Void();
			returnVal->setIsTemp(true);
			return returnVal;
		}
		throw new SyntaxException();
	}

	return nullptr;
}

Type* Parser::getType(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap)
{
	Type* returnVal = nullptr;
	if (Helper::isInteger(str))
	{
		int val = std::stoi(str);
		returnVal = new Integer(val);
	}
	else if (Helper::isBoolean(str))
	{
		returnVal = new Boolean(str == "True");
	}
	else if (Helper::isString(str))
	{
		if (str.length() == 3)
			returnVal = new Character(str[1]);
		else
			returnVal = new String(str.substr(1, str.length() - 2));
	}
	else if (Float::isFloat(str))
	{
		returnVal = new Float(std::stod(str));
	}
	else if (str[0] == '[' && str[str.length() - 1] == ']')
	{
		std::string insideStr = str.substr(1, str.length() - 2);
		returnVal = new List(stringToVector(insideStr));
	}
	return returnVal;
}

bool Parser::isLegalVarName(const std::string& str)
{
	if (!(Helper::isLetter(str[0]) || Helper::isUnderscore(str[0])))
		return false;

	for (unsigned int i = 1; i < str.length(); i++)
	{
		if (!(Helper::isDigit(str[i]) || Helper::isLetter(str[i]) || Helper::isUnderscore(str[i])))
		{
			return false;
		}
	}
		
	for each (std::string reservedWord in reservedWords)
	{
		if (!std::strcmp(reservedWord.c_str(), str.c_str()))
		{
			return false;
		}
	}
		
	return true;
}
bool Parser::isLegalFuncCall (const std::string& str)
{
	std::size_t lBracket, rBracket;
	rBracket = str.find_last_of(')');
	lBracket = str.find_last_of('(');
	if (lBracket == std::string::npos || rBracket == std::string::npos)
		return false;
	std::string funcName = str.substr(0, lBracket);

	if (funcName == "" || (!isLegalVarName(funcName) && !isLegalIndexer(funcName)))
		return false;

	std::string parametersStr = str.substr(lBracket + 1, rBracket - lBracket - 1);
	if (parametersStr == "")
		return true;

	std::vector<std::string> parameters = getKeyWords(parametersStr);
	for (auto paramater : parameters)
	{
		if (!isLegalType(paramater) && !isLegalVarName(paramater) && !isLegalComplex(paramater) && !isLegalFuncCall(paramater) && !isLegalIndexer)
			return false;
	}
	return true;
}
bool Parser::isLegalAssigment(const std::string& str)
{
	int equSign = str.find_first_of('=');

	if (equSign == std::string::npos || equSign == 0 || equSign == str.size() - 1 || str[equSign + 1] == '=' || str[equSign - 1] == '!'
		|| str[equSign - 1] == '<' || str[equSign - 1] == '>')
	{
		return false;
	}

	std::string varName = str.substr(0, equSign);
	std::string valueStr = str.substr(equSign + 1);

	Helper::rtrim(varName);
	Helper::ltrim(valueStr);

	if (varName.length() == 0 || valueStr.length() == 0)
		return false;

	if (!isLegalVarName(varName))
		throw new SyntaxException();

	if (!isLegalType(valueStr) && !isLegalVarName(valueStr) && !isLegalFuncCall(valueStr) && !isLegalComplex(valueStr) && !isLegalIndexer(valueStr))
		throw new SyntaxException();

	return true;
}
bool Parser::isLegalReturn(const std::string& str)
{
	int firstSpace = str.find_first_of(" \t");
	if (firstSpace == std::string::npos)
		return false;
	std::string firstWord = str.substr(0, firstSpace);
	if (firstWord != "return")
		return false;

	std::string valueStr = str.substr(firstSpace + 1);
	Helper::trim(valueStr);

	if (isLegalVarName(valueStr) || isLegalType(valueStr) || isLegalFuncCall(valueStr) || isLegalComplex(valueStr) || isLegalIndexer(valueStr))
		return true;

	throw new SyntaxException();
}
bool Parser::isLegalComplex(const std::string& str)
{
	std::vector<std::string> keyWords = getComplexWords(str);
	if (keyWords.empty())
		return false;
	for (std::string keyWord : keyWords)
	{
		if (!isLegalType(keyWord) && !isLegalVarName(keyWord) && !isLegalFuncCall(keyWord))
			return false;
	}
	return true;
}
bool Parser::isLegalType(const std::string& str)
{
	if (Helper::isInteger(str) || Helper::isBoolean(str) || Helper::isString(str) || Float::isFloat(str))
		return true;
	else if (str[0] == '[' && str[str.length() - 1] == ']')
	{
		std::vector<std::string> items = getKeyWords(str.substr(1, str.size() - 2));
		for (auto item : items)
		{
			if (!isLegalType(item) && !isLegalVarName(item) && !isLegalComplex(item) && !isLegalFuncCall(item) && !isLegalIndexer(item))
				return false;
		}
		return true;
	}
	return false;
}
bool Parser::isLegalIndexer(const std::string& str)
{
	/*std::size_t lBracket, rBracket;
	lBracket = str.find_first_of('[');
	rBracket = str.find_last_of(']');
	if (lBracket == std::string::npos || rBracket == std::string::npos)
		return false;
	std::string varName = str.substr(0, lBracket);

	if (varName == "" || (!isLegalVarName(varName) && !isLegalIndexer(varName) && !isLegalFuncCall(varName) && !isLegalType(varName) && !isLegalComplex(varName)))
		return false;

	std::string index = str.substr(lBracket + 1, rBracket - lBracket - 1);
	if (index == "")
		return false;

	return isLegalType(index) || isLegalVarName(index) || isLegalFuncCall(index) || isLegalComplex(index) || isLegalIndexer(index);*/

	return false;
}


Type* Parser::getVariableValue(const std::string &str, std::unordered_map<std::string, Type*>* varMap)
{
	if (varMap == nullptr)
		varMap = &_variables;
	std::unordered_map<std::string, Type*>::iterator got = varMap->find(str);
	if (got == varMap->end())
	{
		if (varMap != &_variables)
			return getVariableValue(str);
		else 
			return nullptr;
	}
	return got->second;
}
//Type* Parser::getValueAtIndex(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap)
//{
//
//}
//void Parser::setValueAtIndex(const std::string &str, Type* val, std::unordered_map<std::string, Type*>* localVarMap)
//{
//
//}
//std::pair<Type*, Type*> Parser::getVarAndIndx(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap)
//{
//	int lBracket = str.find_first_of('[');
//	int rBracket = str.find_last_of(']');
//	std::string varName = str.substr(0, lBracket);
//	std::string index = str.substr(lBracket + 1, rBracket - lBracket - 1);
//	Type* var = getValue(varName)
//}
//std::pair<int, int> Parser::getBrackets(std::string str)
//{
//	std::vector<char> openers;
//	int firstBracket;
//	for (int i = 0; i < str.size() - 1; i++)
//	{
//		if (openers.back() == '\'')
//		{
//			if (str[i] == '\'')
//				openers.pop_back();
//			continue;
//		}
//		if (openers.back() == '\"')
//		{
//			if (str[i] == '\"')
//				openers.pop_back();
//			continue;
//		}
//		switch ( str[i])
//		{
//		default:
//			break;
//		}
//
//	}
//}
Type* Parser::getValue(std::string valueStr, std::unordered_map<std::string, Type*>* varMap)
{
	Type* value;
	if (isLegalVarName(valueStr))
	{
		value = getVariableValue(valueStr, varMap);

		if (value == nullptr)
			throw new NameErrorException(valueStr);

		if (value->shouldClone())
			value = value->clone();
	}
	else if (isLegalFuncCall(valueStr))
	{
		value = callFunc(valueStr, varMap);
		if (value == nullptr)
			throw new NameErrorException(getFuncName(valueStr));
	}
	else
	{
		value = getType(valueStr);
		if (value == nullptr)
		{
			if (isLegalComplex(valueStr))
				value = calcComplex(valueStr, varMap);
			else
				throw new SyntaxException();
		}
	}
	return value;
}

void Parser::freeMemory(std::unordered_map<std::string, Type*>* varMap)
{
	if (varMap == nullptr)
		varMap = &_variables;
	for (auto x : *varMap)
		delete x.second;
}

std::vector<Type*> Parser::stringToVector(const std::string &str, std::unordered_map<std::string, Type*>* varMap)
{
	std::vector<Type*> items;
	std::vector<std::string> keyWords = getKeyWords(str);
	for each (std::string keyWord in keyWords)
		items.push_back(getValue(keyWord, varMap));
	return items;
}
std::vector<std::string> Parser::getKeyWords(const std::string &str, char seperator)
{
	if (str == "")
		return std::vector<std::string>();
	char opener = NULL;
	std::vector<std::string> keyWords;
	std::string keyWordStr = "";
	int last = 0;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (opener == NULL)
		{
			if ((str[i] == seperator || (seperator == ' ' && str[i] == '\t')))
			{
				if (!((seperator == ' ' || seperator == '\t') && last == i))
				{
					keyWordStr = str.substr(last, i - last);
					Helper::trim(keyWordStr);
					if (keyWordStr == "")
						throw new SyntaxException();
					keyWords.push_back(keyWordStr);
				}
				last = i + 1;
			}
			else if (str[i] == '\'' || str[i] == '"' || str[i] == '[' || str[i] == '(')
				opener = str[i];
		}
		else if ((opener == '"' && str[i] == '"') || (opener == '\'' && str[i] == '\'') || (opener == '[' && str[i] == ']') || (opener == '(' && str[i] == ')'))
			opener = NULL;
	}

	if (last == str.size() || opener != NULL)
		throw new SyntaxException();

	keyWordStr = str.substr(last);
	Helper::trim(keyWordStr);
	if (keyWordStr == "")
		throw new SyntaxException();
	keyWords.push_back(keyWordStr);

	return keyWords;
}
std::vector<std::string> Parser::getComplexWords(const std::string &str)
{
	bool *legals = new bool[str.size()];
	std::vector<std::string> keyWords;

	for (int i = 0; i < str.size(); i++)
	{
		bool shouldCon = false;
		if (i < str.size() - 1)
		{
			std::string temp = str.substr(i, 2);
			for (std::string op : sizeTwoOps)
			{
				if (op == temp)
				{
					legals[i] = false;
					legals[i + 1] = false;
					i++;
					shouldCon = true;
					break;
				}
			}
			if (shouldCon)
				continue;
		}
		if (str[i] == '\"')
		{
			int last = i;
			legals[i++] = true;
			while (str[i] != '\"' && i <str.size())
				legals[i++] = true;
			legals[i] = true;
			continue;
		}
		if (str[i] == '\'')
		{
			int last = i;
			legals[i++] = true;
			while (str[i] != '\'' && i <str.size())
				legals[i++] = true;
			legals[i] = true;
			continue;
		}
		for (char op : sizeOneOps)
		{
			if (op == str[i])
			{
				legals[i] = false;
				shouldCon = true;
				break;
			}
		}
		if (shouldCon)
			continue;
		for (char op : legalComplex)
		{
			if (op == str[i])
			{
				legals[i] = false;
				shouldCon = true;
				break;
			}
		}
		if (shouldCon)
			continue;
		legals[i] = (str[i] != ' ');
	}
	int last = 0;
	int i = 0;
	for (i; i < str.size(); i++)
	{
		if (legals[i])
		{
			if (!legals[last])
				last = i;
			continue;
		}
		if (legals[last])
		{
			keyWords.push_back(str.substr(last, i - last));
		}
			
		last = i;
	}
	if (last != i - 1 && legals[last])
	{
		keyWords.push_back(str.substr(last, i - last));
	}
	return keyWords;
}

void Parser::makeAssignment(const std::string& str, std::unordered_map<std::string, Type*>* localVarMap)
{
	int equSign = str.find_first_of('=');

	std::string varName = str.substr(0, equSign);
	std::string valueStr = str.substr(equSign + 1);

	Helper::rtrim(varName);
	Helper::ltrim(valueStr);

	Type* value = getValue(valueStr, localVarMap);

	Type* varP = getVariableValue(varName);
	if (varP == nullptr && localVarMap != nullptr)
		(*localVarMap)[varName] = value;
	else
		_variables[varName] = value;
}
void Parser::assignReturnValue(const std::string& str, std::unordered_map<std::string, Type*>* localVarMap)
{
	(*localVarMap)["return"] = getValue(str.substr(str.find_first_of(" \t") + 1), localVarMap);
}
Function* Parser::assignFunc(const std::string &str)
{
	std::vector<std::string> keyWords = getKeyWords(str, ' ');
	std::vector<std::string> parametersNames;
	std::string funcName, parametersStr;
	std::size_t lBracket, rBracket, opener;

	if (keyWords[0] != "def")
		return nullptr;

	while (keyWords.size() > 2)
	{
		keyWords[keyWords.size() - 2] += keyWords[keyWords.size() - 1];
		keyWords.pop_back();
	}

	lBracket = keyWords[1].find_first_of('(');
	rBracket = keyWords[1].find_last_of(')');
	opener = keyWords[1].find_first_of(':');

	if (lBracket == std::string::npos || lBracket == std::string::npos ||  opener != keyWords[1].size() - 1)
		throw new SyntaxException();

	if (keyWords[1].find_first_not_of({ ' ', '\t' }, rBracket + 1) != opener)
		throw new SyntaxException();

	parametersStr = keyWords[1].substr(lBracket + 1, rBracket - lBracket - 1);
	funcName = keyWords[1].substr(0, lBracket);

	if (!isLegalVarName(funcName))
		throw new SyntaxException();

	if (parametersStr.size() != 0)
	{
		parametersNames = getKeyWords(parametersStr);
		for each (std::string paramName in parametersNames)
			if (!isLegalVarName(paramName))
				throw new SyntaxException();
	}

	_variables[funcName] = new Function(parametersNames);
	return (Function*)(_variables[funcName]);
}

std::string Parser::getFuncName(const std::string& str)
{
	std::size_t lBracket = str.find_first_of('(');
	return str.substr(0, lBracket);
}

Type* Parser::callFunc(const std::string &str, std::unordered_map<std::string, Type*>* localVarMap)
{
	std::size_t lBracket, rBracket;
	lBracket = str.find_first_of('(');
	rBracket = str.find_last_of(')');
	std::string funcName = str.substr(0, lBracket);
	std::string parametersStr = str.substr(lBracket + 1, rBracket - lBracket - 1);

	Type* Func = getVariableValue(funcName, localVarMap);

	if (Func == nullptr || Func->getTypeName() != ClassType::FunctionC)
		return nullptr;

	std::vector<Type*> parameters = stringToVector(parametersStr, localVarMap);

	return ((Function*)Func)->run(parameters);
}

Type* Parser::calcComplex(const std::string& str, std::unordered_map<std::string, Type*>* localVarMap)
{
	Type* returnVal = createTree(str, localVarMap)->calcTree();
	if (returnVal == nullptr)
		throw new SyntaxException();
	return returnVal;
}
BinTreeNode* Parser::createTree(const std::string& str, std::unordered_map<std::string, Type*>* localVarMap)
{
	std::string cleanStr = str;
	Helper::trim(cleanStr);
	Type* value;
	int strSize = cleanStr.size();
	if (cleanStr == "")
	{
		throw new SyntaxException();
	}
	if (cleanStr[0] == '(' && cleanStr[strSize - 1] == ')')
		return createTree(cleanStr.substr(1, strSize - 2), localVarMap);
	else if (isLegalVarName(cleanStr))
	{
		value = getVariableValue(cleanStr, localVarMap);

		if (value == nullptr)
			throw new NameErrorException(cleanStr);
	}
	else if ((value = getType(cleanStr)) != nullptr){}
	else if (isLegalFuncCall(cleanStr))
	{
		value = callFunc(cleanStr, localVarMap);
		if (value == nullptr)
			throw new NameErrorException(getFuncName(cleanStr));
	}
	else
	{
		std::vector<std::pair<int, int>> untouchedRanges;
		for (int i = strSize - 1; i >= 0; i--)
		{
			int left = strSize;
			switch (cleanStr[i])
			{
			case '\'':
			case '\"':
				left = cleanStr.find_last_of(cleanStr[i], i - 1);
				break;
			case ')':
				left = cleanStr.find_last_of('(', i - 1);
				break;
			case '[':
				left = cleanStr.find_last_of(']', i - 1);
				break;
			default:
				break;
			}
			if (left == std::string::npos)
				throw new SyntaxException();
			else if (left != strSize)
			{
				untouchedRanges.push_back(std::pair<int, int>(left, i));
				i = left;
			}
		}
		int rangeIdx = 0;
		// first rate operators = *, /, %
		for (int i = strSize - 1; i >= 0; i--)
		{
			if (rangeIdx < untouchedRanges.size())
			{
				if (i == untouchedRanges[rangeIdx].second)
				{
					i = untouchedRanges[rangeIdx++].first;
					continue;
				}
			}
			if (cleanStr[i] == '*' || cleanStr[i] == '/' || cleanStr[i] == '%')
			{
				Operand* myOp = new Operand((OperandType)cleanStr[i]);
				BinTreeNode* left = createTree(cleanStr.substr(0, i), localVarMap);
				BinTreeNode* right = createTree(cleanStr.substr(i + 1), localVarMap);
				return new BinTreeNode(myOp, left, right);
			}
		}
		// second rate operators = +, -
		rangeIdx = 0;
		for (int i = strSize - 1; i >= 0; i--)
		{
			if (rangeIdx < untouchedRanges.size())
			{
				if (i == untouchedRanges[rangeIdx].second)
				{
					i = untouchedRanges[rangeIdx++].first;
					continue;
				}
			}
			if (cleanStr[i] == '+' || cleanStr[i] == '-')
			{
				Operand* myOp = new Operand((OperandType)cleanStr[i]);
				BinTreeNode* left = createTree(cleanStr.substr(0, i), localVarMap);
				BinTreeNode* right = createTree(cleanStr.substr(i + 1), localVarMap);
				return new BinTreeNode(myOp, left, right);
			}
		}
		// comparsion opreators
		rangeIdx = 0;
		for (int i = strSize - 1; i >= 0; i--)
		{
			if (rangeIdx < untouchedRanges.size())
			{
				if (i == untouchedRanges[rangeIdx].second)
				{
					i = untouchedRanges[rangeIdx++].first;
					continue;
				}
			}
			if (i != 0)
			{
				std::string temp = cleanStr.substr(i - 1, 2);
				for (std::string op : sizeTwoOps)
				{
					if (op == temp)
					{
						Operand* myOp = new Operand((OperandType)(op[0] * op[1] % 256));
						BinTreeNode* left = createTree(cleanStr.substr(0, i - 1), localVarMap);
						BinTreeNode* right = createTree(cleanStr.substr(i + 1), localVarMap);
						return new BinTreeNode(myOp, left, right);
					}
				}
			}
			if (cleanStr[i] == '>' || cleanStr[i] == '<')
			{
				Operand* myOp = new Operand((OperandType)cleanStr[i]);
				BinTreeNode* left = createTree(cleanStr.substr(0, i), localVarMap);
				BinTreeNode* right = createTree(cleanStr.substr(i + 1), localVarMap);
				return new BinTreeNode(myOp, left, right);
			}
			
		}
	}
	return new BinTreeNode(value);
}