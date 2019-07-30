#pragma once

#ifdef PH_WINDOWS
constexpr char PH_PATH_SEPARATOR = '\\';
#else
constexpr char PH_PATH_SEPARATOR = '/';
#endif

#include <string>

namespace ph {
	namespace FilePath {
		std::string toModuleName(const std::string& filePath);

		std::string toFilename(const std::string& path, char separator = PH_PATH_SEPARATOR);
	}
}
