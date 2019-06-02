#ifndef POPHEAD_UTILITIES_PARSER_H_
#define POPHEAD_UTILITIES_PARSER_H_

#ifdef _WIN32
#define PH_PATH_SEPARATOR '\\'
#else
#define PH_PATH_SEPARATOR '/'
#endif

#include <string>

namespace PopHead {
namespace Utilities {

namespace Parser {
	std::string toModuleName(const std::string& sourceFilePath);

	std::string toFilename(const std::string& path, char separator = PH_PATH_SEPARATOR);
}

}
}

#endif
