#include "log.hpp"


using PopHead::Logs::LogType;
using PopHead::Logs::Log;
using PopHead::Logs::ModuleID;


Log::Log(LogType logType, ModuleID moduleID, const std::string message)
	: type(logType), moduleID(moduleID), message(message)
{}
