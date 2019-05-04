#include "logSettings.hpp"

using PopHead::Logs::LogSettings;
using PopHead::Logs::ModuleID;
using PopHead::Logs::LogType;

LogSettings::LogSettings()
	: mShouldLogIntoFile(true)
	, mShouldLogIntoConsole(true)
{
}

void LogSettings::setWritingLogs(bool enabled)
{
	mShouldLogIntoConsole = enabled;
	mShouldLogIntoFile = enabled;
}

void LogSettings::setWriteLogsOnlyFromCertainModules(std::vector <ModuleID> moduleID)
{
}

void LogSettings::setWritingLogsFromEachModule()
{
}

void LogSettings::setWritingLogsFromEachLogType()
{

}

void LogSettings::setWritingLogsOnlyFromCertainTypes(std::vector <LogType> logType)
{
}

void LogSettings::setWritingEachLog()
{
}

bool LogSettings::shouldThisLogBeWrittenIntoConsole()
{
	return true;
}

bool LogSettings::shouldThisLogBeWrittenIntoFile()
{
	return true;
}