#include "Logs.hpp"

namespace PopHead
{


void LOG(LogType logType, ModuleID moduleID, std::string message)
{
	PopHead::Logs::Log log(logType, moduleID, message);
	auto& logManager = PopHead::Logs::LogManager::getLogManager();
	logManager.writeLog(log);
}


}