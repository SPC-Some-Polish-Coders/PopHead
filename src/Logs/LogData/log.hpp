#ifndef POPHEAD_LOGS_LOG_H_
#define POPHEAD_LOGS_LOG_H_

#include "moduleID.hpp"
#include "logType.hpp"
#include <string>

namespace PopHead {
namespace Logs {

class Log
{
public:
	Log(LogType logType, ModuleID moduleID, const std::string message);
	LogType type;
	ModuleID moduleID;
	std::string message;
};

}}

#endif // !POPHEAD_LOG_LOG_H_