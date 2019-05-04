#ifndef POPHEAD_LOGS_H_
#define POPHEAD_LOGS_H_

#include "Logs/logManager.hpp"
#include "Logs/logType.hpp" 

using PopHead::Logs::Log;
using PopHead::Logs::LogType;
using PopHead::Logs::ModuleID;

void LOG(const PopHead::Logs::Log& log)
{
	auto& logManager = PopHead::Logs::LogManager::getLogManager();
	logManager.writeLog(log);
}

#endif // !POPHEAD_LOGS_H_

