#ifndef POPHEAD_UTILITIES_PARSER_H_
#define POPHEAD_UTILITIES_PARSER_H_

#ifdef _WIN32
#define PH_PATH_SEPARATOR std::string("/")
#else
#define PH_PATH_SEPARATOR std::string("\\")
#endif

#include <string>

namespace PopHead {
namespace Utilities {

namespace Parser {
	std::string toModuleName(const std::string& sourceFilePath);
}

}
}

#endif
