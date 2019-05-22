#include "parser.hpp"
#include "Logs/assert.hpp"
#include <iostream>
#include <string>

std::string PopHead::Utilities::Parser::toModuleName(const std::string& sourceFilePath)
{
	// WARNING: Don't use PH_EXCEPTION or PH_LOG here becouse they are using this method, so it can result in recursion

	const std::string fullPath = "PopHead" + PH_PATH_SEPARATOR + "src";
	std::size_t begin = sourceFilePath.find(fullPath);
	if (begin == std::string::npos) {
		std::cout << "[Parser::toModuleName] Module location cannot be found" << std::endl;
		throw std::runtime_error("[Parser::toModuleName] Module location cannot be found");
	}
	if (sourceFilePath.find(fullPath, begin + 1) != std::string::npos) {
		std::cout << "[Parser::toModuleName] Move folder with project to another location" << std::endl;
		throw std::runtime_error("[Parser::toModuleName] Move folder with project to another location");
	}
	begin += fullPath.size() + 1;
	if (begin >= sourceFilePath.size()) {
		std::cout << "[Parser::toModuleName] There should be at least one letter after source path" << std::endl;
		throw std::runtime_error("[Parser::toModuleName] There should be at least one letter after source path");
	}

	const std::size_t end = sourceFilePath.find(PH_PATH_SEPARATOR, begin + 1);
	if (end == std::string::npos)
		return "None";
	else
		return sourceFilePath.substr(begin, end - begin);
}
