#ifndef POPHEAD_LOGS_H_
#define POPHEAD_LOGS_H_

#include "Logs/logger.hpp"
#include "Logs/LogData/logType.hpp"
#include <string>

using PopHead::Logs::Log;
using PopHead::Logs::LogType;
using PopHead::Logs::ModuleID;

namespace PopHead 
{
	void LOG(LogType, ModuleID, std::string message);
}

#endif // !POPHEAD_LOGS_H_
