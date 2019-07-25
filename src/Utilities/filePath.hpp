#pragma once

#ifdef PH_WINDOWS
#define PH_PATH_SEPARATOR '\\'
#else
#define PH_PATH_SEPARATOR '/'
#endif

#include <string>

namespace ph {

namespace FilePath {
	std::string toModuleName(std::string path);

	std::string toFilename(const std::string& path, char separator = PH_PATH_SEPARATOR);
}

}
