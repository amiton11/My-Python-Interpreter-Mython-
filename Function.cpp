
#include "Function.h"

Function::Function(std::vector<std::string> parametersNames, std::vector<std::string> lines) : Function(parametersNames)
{
	_lines = lines;
}
Function::Function(std::vector<std::string> parametersNames)
{
	_parametersNames = parametersNames;
	_myType = ClassType::FunctionC;
}

std::vector<std::string> Function::getLines() const
{
	return _lines;
}
void Function::setLines(std::vector<std::string> lines)
{
	_lines = std::vector<std::string>(lines.begin(), lines.end());
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
	for each (std::string line in _lines)
	{
		try
		{
			Type* returnVal = Parser::parseString(line, &_localVars);
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
		if (Parser::getVariableValue("return", &_localVars) != nullptr)
		{
			Type* returnVar = _localVars["return"]->clone();
			Parser::freeMemory(&_localVars);
			return returnVar;
		}
	}
	Parser::freeMemory(&_localVars);
	return new Void();
}

bool Function::parseInto()
{
	std::string input_string;

	// get new command from user
	std::cout << ">>> ";
	std::getline(std::cin, input_string);

	while (input_string != "" && (input_string[0] == ' ' || input_string[0] == '\t'))
	{
		Helper::trim(input_string);
		if (!(Parser::isLegalVarName(input_string) || Parser::isLegalFuncCall(input_string) || 
			Parser::isLegalAssigment(input_string) || Parser::isLegalReturn(input_string) ||
			Parser::isLegalComplex(input_string)))
		{
			Type* tempType = Parser::getType(input_string);
			if (tempType == nullptr)
				return false;
			delete tempType;
		}
		_lines.push_back(input_string);

		std::cout << ">>> ";
		std::getline(std::cin, input_string);
	}
	if (input_string != "")
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