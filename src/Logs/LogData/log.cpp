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

	const char* const PROJECT_SOURCE_PATH = "PopHead\\src";
	std::size_t begin = moduleName.find(PROJECT_SOURCE_PATH);
	PH_ASSERT_WITH_COUT(begin != std::string::npos, "Module location cannot be found");
	PH_ASSERT_WITH_COUT(moduleName.find(PROJECT_SOURCE_PATH, begin + 1) == std::string::npos, "Move folder with project to another location");
	begin += std::strlen(PROJECT_SOURCE_PATH) + 1;
	PH_ASSERT_WITH_COUT(begin < moduleName.size(), "There should be at least one letter after source path");

	std::size_t end = moduleName.find("\\", begin + 1);
	if (end == std::string::npos)
		moduleName = "None";
	else
		moduleName = moduleName.substr(begin, end - begin);
}
