
#include "Function.h"

Function::Function(std::vector<std::string> parametersNames, std::vector<TreeNode*> lines) : Function(parametersNames)
{
	_lines = lines;
}
Function::Function(std::vector<std::string> parametersNames)
{
	_parametersNames = parametersNames;
	_myType = ClassType::FunctionC;
}

std::vector<TreeNode*> Function::getLines() const
{
	return _lines;
}
void Function::setLines(std::vector<TreeNode*> lines)
{
	_lines = std::vector<TreeNode*>(lines.begin(), lines.end());
}
std::vector<std::string> Function::getParameters() const
{
	return _parametersNames;
}
void Function::setParameters(std::vector<std::string> parametersNames)
{
	_parametersNames = std::vector<std::string>(parametersNames.begin(), parametersNames.end());
}

Type* Function::run(std::vector<Type*> parameters)
{
	if (parameters.size() != _parametersNames.size())
		throw new ParameterException();
	for (int i = 0; i < parameters.size(); i++)
		_localVars[_parametersNames[i]] = parameters[i];
	for each (TreeNode* line in _lines)
	{
		try
		{
			if (line == nullptr)
				break;
			if (line->getValue() == "return")
			{
				if (line->childCount() != 1 || line->getChildAt(0) == nullptr)
					break;
				return Parser::runCommand(line->getChildAt(0), &_localVars);
			}
			Type* returnVal = Parser::runCommand(line, &_localVars);
			if (returnVal->isPrintable())
				std::cout << returnVal->toString() << std::endl;
			if (returnVal->getIsTemp())
				delete returnVal;
		}
		catch (InterperterException* ex)
		{
			std::cout << ex->what() << std::endl;
			return new Void();
		}
	}
	Parser::freeMemory(&_localVars);
	return new Void();
}

bool Function::parseInto(std::string preIndent)
{
	std::string input_string;

	// get new command from user
	std::cout << ">>> ";
	std::getline(std::cin, input_string);
	
	std::string curIndent = "";

	while (input_string.size() > preIndent.size() + (curIndent.size() == 0 ? 1 : curIndent.size()) && input_string.substr(0, preIndent.size()) == preIndent
		&& (curIndent == "" || input_string.substr(preIndent.size(), curIndent.size()) == curIndent)) 
	{// while line starts with previous indents and either has a certain new indent if first line, or matches also the indents of previous line in function
		TreeNode* curLine;
		std::string unindentLine = input_string.substr(preIndent.size(), input_string.size() - preIndent.size());
		if (curIndent == "")
		{ 
			auto firstChar = unindentLine.find_first_not_of(" \t");
			if (firstChar == unindentLine.npos)
				return false;
			if (firstChar == 0)
				break;
			curIndent = unindentLine.substr(0, firstChar);
		}
		Helper::trim(unindentLine);
		if (Parser::isLegalReturn(unindentLine))
			curLine = new TreeNode("return", Parser::getComplexTree(Parser::getCleanStr(unindentLine.substr(7, unindentLine.size() - 7))));
		else
			curLine = Parser::getComplexTree(Parser::getCleanStr(unindentLine));
		
		_lines.push_back(curLine);

		std::cout << ">>> ";
		std::getline(std::cin, input_string);
	}
	if (input_string != "") // if user wrote new line that doesn't belong to the fucntion -> run it like a regular line
	{
		try
		{
			Type* myType = Parser::parseString(input_string);
			if (myType != nullptr)
			{
				if (myType->isPrintable())
					std::cout << myType->toString() << std::endl;
				if (myType->getIsTemp())
				{
					delete myType;
				}
					
			}
		}
		catch (InterperterException* ex)
		{
			std::cout << ex->what() << std::endl;
		}
	}
	return true;
}

Type* Function::clone() const
{
	return nullptr;
}