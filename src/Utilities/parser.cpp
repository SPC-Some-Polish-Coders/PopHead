#include "parser.hpp"
#include "Logs/assert.hpp"
#include <iostream>
#include <cstring>

std::string PopHead::Utilities::Parser::toModuleName(const std::string& sourceFilePath)
{
	// WARNING: Don't use PH_ASSERT or PH_LOG here becouse they are using this method, so it can result in recursion
	// TODO: Allow this also on Linux (Linux has forward slash instead of backslash)

	const char* const SOURCE_PATH = "PopHead\\src";
	std::size_t begin = sourceFilePath.find(SOURCE_PATH);
	if (begin == std::string::npos) {
		std::cout << "[Parser::toModuleName] Module location cannot be found" << std::endl;
		PH_BREAKPOINT();
	}
	if (sourceFilePath.find(SOURCE_PATH, begin + 1) != std::string::npos) {
		std::cout << "[Parser::toModuleName] Move folder with project to another location" << std::endl;
		PH_BREAKPOINT();
	}
	begin += std::strlen(SOURCE_PATH) + 1;
	if (begin >= sourceFilePath.size()) {
		std::cout << "[Parser::toModuleName] There should be at least one letter after source path" << std::endl;
		PH_BREAKPOINT();
	}

	std::size_t end = sourceFilePath.find("\\", begin + 1);
	if (end == std::string::npos)
		return "None";
	else
		return sourceFilePath.substr(begin, end - begin);
}
