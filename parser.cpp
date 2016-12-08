#include "parser.h"
#include <algorithm>

std::unordered_map<std::string, Type*> _variables;
const std::string reservedWords[] = { "return", "def", "True", "False" };
const std::string sizeTwoOps[] = { "==", "!=", ">=", "<=", "||", "&&" };
const char sizeOneOps[] = { '*', '/', '%', '+', '-', '>', '<' };
const char legalComplex[] = {'[', ']', '(', ')', ','};
const std::pair<const char, const char> openAndClose[] = { { '"', '"' }, { '\'', '\'' }, { '(', ')' }, { '[', ']' }, { '{', '}' } };

Type* Parser::parseString(std::string str)
{
	if ((str.size() > 0) && str[0] == ' ' || str[0] == '\t')
		throw new IndentationException();
	Helper::trim(str);
	Type* returnVal = nullptr;
	if (str.size() == 0)
		return nullptr;
	if (isLegalFuncAssign(str))
	{
		Function* newFunc = assignFunc(getCleanStr(str.substr(4, str.size() - 4))); // 4 is the size of the string "def "
		bool parseSuccesful = newFunc->parseInto();
		if (!parseSuccesful)
			throw new SyntaxException();
		returnVal = new Void();
		returnVal->setIsTemp(true);
		return returnVal;
	}
	TreeNode* curLineTree = getComplexTree(getCleanStr(str));
	returnVal = runCommand(curLineTree);
	if (!returnVal->getIsTemp())
	{
		returnVal = returnVal->clone();
		returnVal->setIsTemp(true);
	}
	return returnVal;
}
//Type* Parser::parseString(std::string str, std::unordered_map<std::string, Type*>* localVarMap) throw()
//{
//	if (str[0] == ' ' || str[0] == '\t')
//		throw new IndentationException();
//	Helper::trim(str);
//	if (str.length() > 0)
//	{
//		Type* returnVal;
//		if (isLegalAssigment(str))
//		{
//			makeAssignment(str, localVarMap);
//			returnVal = new Void();
//			returnVal->setIsTemp(true);
//			return returnVal;
//		}
//
//		if (isLegalFuncCall(str))
//		{
//			returnVal = callFunc(str, localVarMap);
//			if (returnVal == nullptr)
//				throw new NameErrorException(str);
//			return returnVal;
//		}
//		returnVal = getType(str);
//	 	if (returnVal != nullptr)
//		{
//			returnVal->setIsTemp(true);
//			return returnVal;
//		}
//		if (isLegalVarName(str))
//		{
//			returnVal = getVariableValue(str, localVarMap);
//			if (returnVal == nullptr)
//				throw new NameErrorException(str);
//			return returnVal;
//		}
//		if (isLegalIndexer(str))
//		{
//		}
//		if (isLegalComplex(str))
//		{
//			returnVal = calcComplex(str, localVarMap);
//			returnVal->setIsTemp(true);
//			return returnVal;
//		}
//		if (localVarMap != nullptr && isLegalReturn(str))
//		{
//			assignReturnValue(str, localVarMap);
//			returnVal = new Void();
//			returnVal->setIsTemp(true);
//			return returnVal;
//		}
//		Function* newFunc = assignFunc(str);
//		if (newFunc != nullptr)
//		{
//			bool parseSuccesful = newFunc->parseInto();
//			if (!parseSuccesful)
//				throw new SyntaxException();
//			returnVal = new Void();
//			returnVal->setIsTemp(true);
//			return returnVal;
//		}
//		throw new SyntaxException();
//	}
//
//	return nullptr;
//}

std::string Parser::getCleanStr(std::string &str)
{
	std::string cleanStr = "";
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			int closer = findCloser(str[i], str[i], str, i + 1);
			if (closer == -1)
				throw new InterperterException();
			cleanStr += str.substr(i, closer - i + 1);
			i = closer;
			continue;
		}
		if (str[i] == ' ')
			continue;
		cleanStr += str[i];
	}
	return cleanStr;
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
	return returnVal;
}


bool Parser::isLegalVarName(const std::string& str)
{
	if (!(Helper::isLetter(str[0]) || Helper::isUnderscore(str[0])))
		return false;

	for (unsigned int i = 1; i < str.length(); i++)
		if (!(Helper::isDigit(str[i]) || Helper::isLetter(str[i]) || Helper::isUnderscore(str[i])))
			return false;
		
	for each (std::string reservedWord in reservedWords)
		if (reservedWord == str)
			return false;
		
	return true;
}
bool Parser::isLegalType(const std::string& str)
{
	if (Helper::isInteger(str) || Helper::isBoolean(str) || Helper::isString(str) || Float::isFloat(str))
		return true;
	return false;
}
bool Parser::isLegalReturn(const std::string& str)
{
	int firstSpace = str.find_first_of(" \t");
	if (firstSpace == std::string::npos)
		return false;
	std::string firstWord = str.substr(0, firstSpace);
	if (firstWord == "return")
		return true;

	return false;
}
bool Parser::isLegalFuncAssign(const std::string& str)
{
	int firstSpace = str.find_first_of(" \t");
	if (firstSpace == std::string::npos)
		return false;
	std::string firstWord = str.substr(0, firstSpace);
	if (firstWord == "def")
		return true;

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
void Parser::freeMemory(std::unordered_map<std::string, Type*>* varMap)
{
	if (varMap == nullptr)
		varMap = &_variables;
	for (auto x : *varMap)
		delete x.second;
}

Type* Parser::runCommand(TreeNode* commandTree, std::unordered_map<std::string, Type*>* varMap)
{
	if (commandTree == nullptr)
		throw new InterperterException();
	Type *returnVal, *assignVal, *var, *index, *leftVal, *rightVal;
 	std::string curCmd = commandTree->getValue();
	TreeNode *valTree, *varTree;
	Sequence* varSeq;
	std::vector<Type*> indexVec;
	switch (curCmd.size())
	{
	case 1:
		switch (curCmd[0])
		{
		case '=':
			if (commandTree->getChilds().size() != 2)
				throw new InterperterException();
			valTree = commandTree->getChildAt(1);
			varTree = commandTree->getChildAt(0);
			assignVal = runCommand(valTree);
			
			if (varTree->getValue() == "[]*") // if indexer assigner
			{
				if (varTree->childCount() != 2)
					throw new SyntaxException();
				var = runCommand(varTree->getChildAt(0), varMap);
				if (!var->isSequence())
					throw new SyntaxException();
				varSeq = (Sequence*)var;
				index = runCommand(varTree->getChildAt(1), varMap);
				varSeq->setAtIndex(index, assignVal);
				return getTempVoid();
			}

			if (isLegalVarName(varTree->getValue()) && !varTree->hasChilds()) // if variable is valid
			{
				makeAssignment(varTree->getValue(), assignVal, varMap);
				return getTempVoid();
			}

			throw new InterperterException();
			break;
		case '>':
			if (commandTree->getChilds().size() != 2)
				throw new SyntaxException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) > rightVal;
			break;
		case '<':
			if (commandTree->getChilds().size() != 2)
				throw new SyntaxException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) < rightVal;
			break;
		case '+':
			if (commandTree->getChilds().size() != 2)
				throw new SyntaxException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) + rightVal;
			break;
		case '-':
			if (commandTree->getChilds().size() != 2)
				throw new SyntaxException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) - rightVal;
			break;
		case '*':
			if (commandTree->getChilds().size() != 2)
				throw new SyntaxException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) * rightVal;
			break;
		case '/':
			if (commandTree->getChilds().size() != 2)
				throw new SyntaxException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) / rightVal;
			break;
		case '%':
			if (commandTree->getChilds().size() != 2)
				throw new SyntaxException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) % rightVal;
			break;
		case '!':
			if (commandTree->getChilds().size() != 1)
				throw new SyntaxException();
			returnVal = runCommand(commandTree->getChildAt(0), varMap);
			return !(*returnVal);
			break;
		default:
			returnVal = getDefaultVal(curCmd, varMap);
			if (returnVal == nullptr)
				throw new SyntaxException();
			return returnVal;
			break;
		}
		break;
	case 2:
		if (curCmd[1] == '=')
		{
			if (curCmd[0] == '*' || curCmd[0] == '/' || curCmd[0] == '%' || curCmd[0] == '+' || curCmd[0] == '-')
			{
				if (commandTree->getChilds().size() != 2)
					throw new SyntaxException();
				valTree = commandTree->getChildAt(1);
				varTree = commandTree->getChildAt(0);
				assignVal = runCommand(varTree);
				returnVal = runCommand(valTree);
				switch (curCmd[0])
				{
				case '*':
					assignVal = (*assignVal) * returnVal;
					break;
				case '/':
					assignVal = (*assignVal) / returnVal;
					break;
				case '+':
					assignVal = (*assignVal) + returnVal;
					break;
				case '-':
					assignVal = (*assignVal) - returnVal;
					break;
				case '%':
					assignVal = (*assignVal) % returnVal;
					break;
				}
				if (varTree->getValue() == "[]*") // if indexer assigner
				{
					if (varTree->childCount() != 2)
						throw new SyntaxException();
					var = runCommand(varTree->getChildAt(0), varMap);
					if (!var->isSequence())
						throw new SyntaxException();
					varSeq = (Sequence*)var;
					index = runCommand(varTree->getChildAt(1), varMap);
					varSeq->setAtIndex(index, assignVal);
					return getTempVoid();
				}
				if (isLegalVarName(varTree->getValue()) && !varTree->hasChilds()) // if variable is valid
				{
					makeAssignment(varTree->getValue(), assignVal, varMap);
					return getTempVoid();
				}

				throw new InterperterException();
			}
			else if (curCmd[0] == '=' || curCmd[0] == '!' || curCmd[0] == '>' || curCmd[0] == '<')
			{
				if (commandTree->getChilds().size() != 2)
					throw new InterperterException();
				leftVal = runCommand(commandTree->getChildAt(0), varMap);
				rightVal = runCommand(commandTree->getChildAt(1), varMap);
				switch (curCmd[0])
				{
				case '=':
					return (*leftVal) == rightVal;
					break;
				case '!':
					return (*leftVal) != rightVal;
					break;
				case '>':
					return (*leftVal) >= rightVal;
					break;
				case '<':
					return (*leftVal) <= rightVal;
					break;
				}
			}
			else
			{
				throw new SyntaxException();
			}
		}
		else if (curCmd == "||")
		{
			if (commandTree->getChilds().size() != 2)
				throw new InterperterException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) || rightVal;
		}
		else if (curCmd == "&&")
		{
			if (commandTree->getChilds().size() != 2)
				throw new InterperterException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap);
			rightVal = runCommand(commandTree->getChildAt(1), varMap);
			return (*leftVal) && rightVal;
		}
		else if (curCmd == "[]") // list
		{
			returnVal = new List();
			for each (auto child in commandTree->getChilds())
			{
				if (child == nullptr)
					throw new SyntaxException();
				((List*)returnVal)->push(runCommand(child, varMap));
			}
			return returnVal;
		}
		else if (curCmd == "()") // function call
		{
			if (commandTree->getChilds().size() != 2)
				throw new InterperterException();
			leftVal = runCommand(commandTree->getChildAt(0), varMap); // function name
			rightVal = runCommand(commandTree->getChildAt(1), varMap); // parameters
			if (leftVal->getTypeName() != ClassType::FunctionC)
				throw new SyntaxException();
			if (rightVal->getTypeName() != ClassType::ListC)
				throw new SyntaxException();
			return ((Function*)leftVal)->run(((List*)rightVal)->getValue());
		}
		else if (curCmd == "{}") // dictionary
		{
			throw new SyntaxException(); // should creare a dictionary instead
		}
		else
		{
			returnVal = getDefaultVal(curCmd, varMap);
			if (returnVal == nullptr)
				throw new SyntaxException();
			return returnVal;
		}
		break;
	default:
		if (curCmd == "[]*")
		{
			if (commandTree->childCount() != 2)
				throw new SyntaxException();
			var = runCommand(commandTree->getChildAt(0), varMap);
			if (!var->isSequence())
				throw new SyntaxException();
			varSeq = (Sequence*)var;
			indexVec = getIndexRange(commandTree->getChildAt(1), varMap);
			return (*varSeq)[indexVec];
		}
		returnVal = getDefaultVal(curCmd, varMap);
		if (returnVal == nullptr)
			throw new SyntaxException();
		return returnVal;
		break;
	}
}

Type* Parser::getDefaultVal(std::string& str, std::unordered_map<std::string, Type*>* varMap)
{
	// check if current string is a variable
	if (isLegalVarName(str))
		return getVariableValue(str, varMap);
	// check if current command is a raw simple 
	if (isLegalType(str))
		return getType(str, varMap);
	return nullptr;
}

std::vector<Type*> Parser::getIndexRange(TreeNode* indexTree, std::unordered_map<std::string, Type*>* varMap)
{
	std::vector<Type*> indexVec;
	indexRangeRecursive(indexTree, indexVec, varMap);
	return indexVec;
}
void Parser::indexRangeRecursive(TreeNode* indexTree, std::vector<Type*>& indexVec, std::unordered_map<std::string, Type*>* varMap)
{
	if (indexTree == nullptr)
	{
		indexVec.push_back(getTempVoid());
		return;
	}
	std::string curCmd = indexTree->getValue();
	if (curCmd == ":")
	{
		indexRangeRecursive(indexTree->getChildAt(0), indexVec, varMap);
		indexRangeRecursive(indexTree->getChildAt(1), indexVec, varMap);
	}
	else if (curCmd == "")
		indexVec.push_back(getTempVoid());
	else
		indexVec.push_back(runCommand(indexTree, varMap));
}

Void* Parser::getTempVoid()
{
	Void* temp = new Void();
	temp->setIsTemp(true);
	return temp;
}

TreeNode* Parser::getComplexTree(const std::string &str)
{
	if (str == "")
		return nullptr;
	// check if all string is between parentheses
	if (str[0] == '(' && findCloser('(', ')', str, 1) == str.size() - 1)
		return getComplexTree(str.substr(1, str.size() - 2));
	// check if current string is a list
	if (str[0] == '[' && findCloser('[', ']', str, 1) == str.size() - 1)
		return getInsideTree(str.substr(1, str.size() - 2), std::string("[]"));
	// check if current string is a dictionary
	if (str[0] == '{' && findCloser('{', '}', str, 1) == str.size() - 1)
		return getInsideTree(str.substr(1, str.size() - 2), std::string("{}"));
	// check for openers and closers inside the code
	auto insideRange = findInsideRange(str);
	int i, vecIdx;
	// check for assigners and for ':' seprator
	for (i = str.size() - 1, vecIdx = insideRange.size() - 1; i >= 0; i--)
	{
		if (str[i] == '=')
		{
			if (i > 0)
			{
				if (str[i - 1] == '=' || str[i - 1] == '!' || str[i - 1] == '>' || str[i - 1] == '<')
				{
					i -= 1;
					continue;
				}
				if (str[i - 1] == '*' || str[i - 1] == '/' || str[i - 1] == '+' || str[i - 1] == '-' || str[i - 1] == '%')
					return new TreeNode(str.substr(i - 1, 2), getComplexTree(str.substr(0, i - 1)), getComplexTree(str.substr(i + 1, str.size() - i - 1)));
			}
			return new TreeNode("=", getComplexTree(str.substr(0, i)), getComplexTree(str.substr(i + 1, str.size() - i - 1)));
		}
		if (str[i] == ':')
			return new TreeNode(":", getComplexTree(str.substr(0, i)), getComplexTree(str.substr(i + 1, str.size() - i - 1)));
		if (vecIdx >= 0 && insideRange[vecIdx].second.second == i)
			i = insideRange[vecIdx--].second.first;
	}
	// check for Comparers
	for (i = str.size() - 1, vecIdx = insideRange.size() - 1; i > 0; i--)
	{
		std::string curOp = str.substr(i - 1, 2);
		for each(std::string compOp in sizeTwoOps)
			if (compOp == curOp)
				return new TreeNode(curOp, getComplexTree(str.substr(0, i - 1)), getComplexTree(str.substr(i + 1, str.size() - i - 1)));
		if (vecIdx >= 0 && insideRange[vecIdx].second.second == i)
			i = insideRange[vecIdx--].second.first;
	}
	// check for + and -
	for (i = str.size() - 1, vecIdx = insideRange.size() - 1; i >= 0; i--)
	{
		if (str[i] == '+' || str[i] == '-')
			return new TreeNode(str.substr(i, 1), getComplexTree(str.substr(0, i)), getComplexTree(str.substr(i + 1, str.size() - i - 1)));
		if (vecIdx >= 0 && insideRange[vecIdx].second.second == i)
			i = insideRange[vecIdx--].second.first;
	}
	// check for *, / and %
	for (i = str.size() - 1, vecIdx = insideRange.size() - 1; i >= 0; i--)
	{
		if (str[i] == '*' || str[i] == '/' || str[i] == '%')
			return new TreeNode(str.substr(i, 1), getComplexTree(str.substr(0, i)), getComplexTree(str.substr(i + 1, str.size() - i - 1)));
		if (vecIdx >= 0 && insideRange[vecIdx].second.second == i)
			i = insideRange[vecIdx--].second.first;
	}
	// check for indexers and function calls
	for (i = insideRange.size() - 1; i >= 0; i--)
	{
		auto curRange = insideRange[i];
		if (curRange.second.first != 0)
		{
			if (curRange.first == '[')
			{
				if (curRange.second.second == str.size() - 1)
					return new TreeNode(std::string("[]*"), getComplexTree(str.substr(0, curRange.second.first)), getComplexTree(str.substr(curRange.second.first + 1, curRange.second.second - curRange.second.first - 1)));
				else
					throw new InterperterException();
			}
			if (curRange.first == '(')
			{
				if (curRange.second.second == str.size() - 1)
					return new TreeNode(std::string("()"), getComplexTree(str.substr(0, curRange.second.first)), getInsideTree(str.substr(curRange.second.first + 1, curRange.second.second - curRange.second.first - 1), std::string("[]")));
				else
					throw new InterperterException();
			}
		}
	}
	// check for ! (not sign)
	for (i = str.size() - 1, vecIdx = insideRange.size() - 1; i >= 0; i--)
	{
		if (str[i] == '!')
			return new TreeNode("!", getComplexTree(str.substr(0, i)));
		if (vecIdx >= 0 && insideRange[vecIdx].second.second == i)
			i = insideRange[vecIdx--].second.first;
	}
	// check if variable or raw value
	if (isLegalVarName(str) || isLegalType(str))
		return new TreeNode(str);

	throw new InterperterException();
}

TreeNode* Parser::getInsideTree(const std::string &str, const std::string &val)
{
	TreeNode* curTree = new TreeNode(val);
	if (str == "")
		return curTree;
	// check for openers and closers inside the code
	auto insideRange = findInsideRange(str);
	int i, vecIdx, last;
	last = 0;
	//check for coma(,)
	for (i = 0, vecIdx = 0; i < str.size(); i++)
	{
		if (str[i] == ',')
		{
			if (i == last)
				throw new InterperterException();
			curTree->pushChild(getComplexTree(str.substr(last, i - last)));
			last = i+1;
		}
		else if (vecIdx < insideRange.size() && insideRange[vecIdx].second.first == i)
			i = insideRange[vecIdx++].second.second;
	}
	curTree->pushChild(getComplexTree(str.substr(last, str.size() - last)));
	return curTree;
}

std::vector<std::pair<char, std::pair<int, int>>> Parser::findInsideRange(const std::string &str)
{
	std::vector<std::pair<char, std::pair<int, int>>> insideRange;
	int opener, closer;
	closer = -1;
	for (int i = 0; i < str.length(); i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (str[i] != openAndClose[j].first)
				continue;
			int closer = findCloser(openAndClose[j], str, i + 1);
			if (closer == -1)
				throw new InterperterException(); // should be an opener exception

			// push a pair containg the data about what opener we talk about, and the range between this opener and his closer
			insideRange.push_back({ openAndClose[j].first, { i, closer } });

			i = closer;
			break;
		}
	}
	return insideRange;
}

int Parser::findCloser(std::pair<const char, const char> openerAndCloser, const std::string &str, int pos)
{
	return findCloser(openerAndCloser.first, openerAndCloser.second, str, pos);
}
int Parser::findCloser(char opener, char closer, const std::string &str, int pos)
{
	int openersCount = 0;
	if (opener != '"' && opener != '\'')
	{
		char curQuote = 0;
		for (int i = pos; i < str.length(); i++)
		{
			if (curQuote == 0)
			{
				if (closer == str[i])
				{
					if (openersCount == 0)
						return i;
					openersCount--;
					continue;
				}
				else if (opener == str[i])
					openersCount++;
				else if ('\'' == str[i] || '"' == str[i])
				{
					curQuote = str[i];
				}
			}
			else
			{
				if (curQuote == str[i])
					curQuote = 0;
			}
		}
	}
	else
	{
		for (int i = pos; i < str.length(); i++)
		{
			if (closer == str[i])
			{
				if (openersCount == 0)
					return i;
				openersCount--;
				continue;
			}
			if (opener == str[i])
				openersCount++;
		}
	}
	return -1;
}

void Parser::makeAssignment(const std::string& varName, Type* assignVal, std::unordered_map<std::string, Type*>* localVarMap)
{
	Type* varP = getVariableValue(varName);
	if (varP == nullptr && localVarMap != nullptr)
		(*localVarMap)[varName] = assignVal;
	else
		_variables[varName] = assignVal;
}

Function* Parser::assignFunc(const std::string &str)
{
	
	TreeNode* AssignTree = getComplexTree(str);
	if (AssignTree == nullptr || AssignTree->getValue() != ":")
		throw new SyntaxException();
	if (AssignTree->getChildAt(1) != nullptr && AssignTree->getChildAt(1)->getValue() != "")
		throw new SyntaxException();
	TreeNode* funcTree = AssignTree->getChildAt(0);
	if (funcTree == nullptr || funcTree->getValue() != "()")
		throw new SyntaxException();
	if (funcTree->getChildAt(0) == nullptr || funcTree->getChildAt(0)->childCount() != 0)
		throw new SyntaxException();
	std::string funcName = funcTree->getChildAt(0)->getValue();
	if (!isLegalVarName(funcName))
		throw new SyntaxException();
	if (getVariableValue(funcName) != nullptr)
		throw new SyntaxException(); // should be a reassign exception
	std::vector<std::string> parametersNames;
	TreeNode* paramTree = funcTree->getChildAt(1);
	if (paramTree == nullptr || paramTree->getValue() != "[]")
		throw new SyntaxException();
	for each (auto parameter in paramTree->getChilds())
	{
		if (parameter == nullptr)
			throw new SyntaxException();
		if (parameter->childCount() != 0) // should change it when I add default value to parameters
			throw new SyntaxException();
		std::string parameterName = parameter->getValue();
		if (parameterName == "" || !isLegalVarName(parameterName))
			throw new SyntaxException();
		parametersNames.push_back(parameterName);
	}
	_variables[funcName] = new Function(parametersNames);
	return (Function*)(_variables[funcName]);
}