#include "Logs/Log.hpp"

using PopHead::Logs::Log;

Log::Log(LogType type, ModuleID moduleID, const std::string& message)
	:type(type)
	, moduleID(moduleID)
	, message(message)
{
}
