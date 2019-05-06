#include "log.hpp"
#include <cstring>
#include <iostream>

using PopHead::Logs::LogType;
using PopHead::Logs::Log;

Log::Log(LogType logType, const char* const filePath, const std::string& message)
	: message(message), moduleName(std::string(filePath)), type(logType)
{
	//G:\PopHead\src\main.cpp
//G:\PopHead\src\States\stateMachine.cpp
	std::size_t begin = moduleName.rfind("PopHead\\src");
	//std::cout << "Position after first find is: " << begin;
	// TODO: PH_ASSERT(begin != std::string::npos, "Module location cannot be found");
	//std::cout << std::strlen("PopHead\\src\\");
	begin += std::strlen("PopHead\\src\\");
	//std::cout << "Position is: " << begin;
	if (moduleName.find("\\", begin) == std::string::npos)
	{
		moduleName = "None";
		std::cout << "NO MODULE\n";
	}
	else
	{
		std::size_t end = moduleName.find("\\", begin);
		if (end == std::string::npos)
			std::cout << "ERROR\n";
		std::cout << begin << '\n';
		std::cout << end << '\n';
		std::cout << moduleName << '\n';
	}

}
