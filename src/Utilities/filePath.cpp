#include "Utilities/filePath.hpp"
#include "Logs/logs.hpp"

#include <string>

namespace ph {

std::string FilePath::toFilename(const std::string& path, char separator)
{
	const std::size_t begin = path.rfind(separator);
	if(begin != std::string::npos)
		return path.substr(begin + 1);
	return path;
}

std::string FilePath::cutFilePathAndFixSeparator(std::string filePath)
{
	auto sourceDirectoryPos = filePath.rfind("src");
	sourceDirectoryPos += 4;
	filePath.erase(0, sourceDirectoryPos);

	if constexpr (PH_PATH_SEPARATOR == '\\')
		for (auto& ch : filePath)
			if (ch == '\\')
				ch = '/';

	return filePath;
}

}
