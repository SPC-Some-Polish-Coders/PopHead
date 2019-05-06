#ifndef POPHEAD_LOGS_LOG_H_
#define POPHEAD_LOGS_LOG_H_

#include "logType.hpp"
#include <string>



namespace PopHead {
namespace Logs {

class Log
{
public:
	Log(LogType logType, const char* const filePath, const std::string& message);
	std::string message;
	std::string moduleName;
	LogType type;
};

}}

#endif // !POPHEAD_LOG_LOG_H_