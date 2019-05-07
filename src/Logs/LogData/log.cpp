#include "log.hpp"
#include "Logs/assert.hpp"
#include <cstring>
#include <iostream>

using PopHead::Logs::LogType;
using PopHead::Logs::Log;

Log::Log(LogType logType, const char* const filePath, const std::string& message)
	: message(message), moduleName(std::string(filePath)), type(logType)
{
	// WARNING: Don't use PH_ASSERT here, becouse it uses Log, so it can cause recursion
	// TODO: Allow this also on Linux (Linux has forward slash instead of backslash)

	const char* const SOURCE_PATH = "PopHead\\src";
	std::size_t begin = moduleName.find(SOURCE_PATH);
	if (begin == std::string::npos) {
		std::cout << "[Log::Log()] Module location cannot be found" << std::endl;
		PH_BREAKPOINT();
	}
	if (moduleName.find(SOURCE_PATH, begin + 1) != std::string::npos) {
		std::cout << "[Log::Log()] Move folder with project to another location" << std::endl;
		PH_BREAKPOINT();
	}
	begin += std::strlen(SOURCE_PATH) + 1;
	if (begin >= moduleName.size()) {
		std::cout << "[Log::Log()] There should be at least one letter after source path" << std::endl;
		PH_BREAKPOINT();
	}
	std::size_t end = moduleName.find("\\", begin + 1);
	if (end == std::string::npos)
		moduleName = "None";
	else
		moduleName = moduleName.substr(begin, end - begin);
}
