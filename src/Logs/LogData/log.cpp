#include "log.hpp"

using PopHead::Logs::LogType;
using PopHead::Logs::Log;

Log::Log(LogType logType, const char* const filePath, const std::string& message)
	: message(message), type(logType)
{
}
