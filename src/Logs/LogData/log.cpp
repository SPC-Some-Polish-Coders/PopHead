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
	//NOTE: We can't use PH_ASSERT here, becouse it uses Log, so it can cause recursion
	//PH_ASSERT(begin != std::string::npos, "Module location cannot be found");
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
