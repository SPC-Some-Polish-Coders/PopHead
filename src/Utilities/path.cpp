#include "Utilities/path.hpp"
#include "Logs/logs.hpp"

#include <string>

namespace ph {

std::string Path::toModuleName(const std::string& filePath)
{
	// WARNING: Logging macros can be used here only for errors, or problems reporting.
	// When used in a place that execute always (for example in the first line) they cause endless recursion.

	std::string sourceCodeDirectory = std::string("PopHead") + PH_PATH_SEPARATOR + "src";
	auto foundIndex = filePath.rfind(sourceCodeDirectory);

	if (foundIndex == std::string::npos)
	{
		sourceCodeDirectory = std::string("PopHead") + PH_PATH_SEPARATOR + "tests";
		foundIndex = filePath.rfind(sourceCodeDirectory);

		PH_ASSERT_UNEXPECTED_SITUATION(foundIndex != std::string::npos, 
									   '(' + filePath + ") - source code files must be either in src/ or tests/ directory");

		return "Tests";
	}

	// we're looking for the first directory name after sourceCodeDirectory
	foundIndex += sourceCodeDirectory.size() + 1;

	PH_ASSERT_UNEXPECTED_SITUATION(foundIndex < filePath.size(),
								   '(' + filePath + ") - probably given only directory without module and file");

	// we retrieve the separator after module's name
	const auto endOfModuleName = filePath.find(PH_PATH_SEPARATOR, foundIndex);

	if (foundIndex < filePath.size() && endOfModuleName == std::string::npos)
		return "MainDirectory";

	return filePath.substr(foundIndex, endOfModuleName - foundIndex);
}

std::string Path::toFilename(const std::string& path, char separator)
{
	const std::size_t begin = path.rfind(separator);
	if(begin != std::string::npos)
		return path.substr(begin + 1);
	return path;
}

}
