#ifndef POPHEAD_UTILITIES_PATH_H_
#define POPHEAD_UTILITIES_PATH_H_

#ifdef _WIN32
#define PH_PATH_SEPARATOR '\\'
#else
#define PH_PATH_SEPARATOR '/'
#endif

#include <string>

namespace ph {

namespace Path {
	std::string toModuleName(const std::string& path);

	std::string toFilename(const std::string& path, char separator = PH_PATH_SEPARATOR);
}

}

#endif
