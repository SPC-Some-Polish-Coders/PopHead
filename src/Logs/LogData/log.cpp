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
	if (moduleName.find("\\", begin) == std::string::npos)
	{
		moduleName = "None";
	}
	else
	{
		std::size_t end = moduleName.find("\\", begin);
		moduleName.erase(end);
		moduleName.erase(0, begin);
		if (end == std::string::npos)
			std::cout << "ERROR\n";
		std::cout << moduleName << '\n';
	}

}
