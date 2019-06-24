#include "Utilities/path.hpp"
#include "Utilities/debug.hpp"
#include <iostream>
#include <string>

namespace ph {

std::string Path::toModuleName(const std::string& path)
{
	// WARNING: Don't use PH_EXCEPTION or PH_LOG here becouse they are using this method, so it can result in recursion

	const std::string searchedPath = std::string("PopHead") + PH_PATH_SEPARATOR + "src";
	std::size_t begin = path.find(searchedPath);
	if (begin == std::string::npos) {
		std::cout << "[Path::toModuleName] Module location cannot be found" << std::endl;
		throw std::runtime_error("[Path::toModuleName] Module location cannot be found");
	}
	// TODO: Replace that with rfind()?
	if (path.find(searchedPath, begin + 1) != std::string::npos) {
		std::cout << "[Path::toModuleName] Move folder with project to another location" << std::endl;
		throw std::runtime_error("[Path::toModuleName] Move folder with project to another location");
	}
	begin += searchedPath.size() + 1;
	if (begin >= path.size()) {
		std::cout << "[Path::toModuleName] There should be at least one letter after source path" << std::endl;
		throw std::runtime_error("[Path::toModuleName] There should be at least one letter after source path");
	}

	const std::size_t end = path.find(PH_PATH_SEPARATOR, begin + 1);
	if (end == std::string::npos)
		return "None";
	else
		return path.substr(begin, end - begin);
}

std::string Path::toFilename(const std::string& path, char separator)
{
	const std::size_t begin = path.rfind(separator);
	if(begin != std::string::npos)
		return path.substr(begin + 1);
	return path;
	/*
		TODO:
		Probably another possible implementation (which is better/faster?)

		const std::size_t begin = path.rfind(separator);
		return path.substr(begin + 1);
	*/
}

}