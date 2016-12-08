#include "Type.h"
#include "InterperterException.h"
#include "IndentationException.h"
#include "parser.h"
#include <iostream>

#define WELCOME "Welcome to Magshimim Python Interperter version 1.0 by "
#define YOUR_NAME "Amit Alon"


int main(int argc,char **argv)
{
	std::cout << WELCOME << YOUR_NAME << std::endl;

	std::string input_string;

	// get new command from user
	std::cout << ">>> ";
	std::getline(std::cin, input_string);
	
	while (input_string != "quit()" && input_string != "exit()")
	{
		// parsing command
		try
		{
			Type* myType = Parser::parseString(input_string);
			if (myType != nullptr)
			{
				if (myType->isPrintable())
					std::cout << myType->toString() << std::endl;
				if (myType->getIsTemp())
					delete myType;
			}
		}
		catch (InterperterException* ex)
		{
			std::cout << ex->what() << std::endl;
		}
		// get new command from user
		std::cout << ">>> ";
		std::getline(std::cin, input_string);
	}

	Parser::freeMemory();

	return 0;
}


