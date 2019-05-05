#include "Logs.hpp"

namespace PopHead
{


void LOG(LogType logType, ModuleID moduleID, std::string message)
{
	PopHead::Logs::Log log(logType, moduleID, message);
	auto& Logger = PopHead::Logs::Logger::getLogger();
	Logger.writeLog(log);
}


}