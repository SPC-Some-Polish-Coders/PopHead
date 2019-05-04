#include "logSettings.hpp"

using PopHead::Logs::LogSettings;
using PopHead::Logs::ModuleID;
using PopHead::Logs::LogType;
using PopHead::Logs::Log;

LogSettings::LogSettings()
	:mShouldLogIntoConsole(true)
	,mShouldLogIntoFile(true)
{
	setWritingEachLog();
}

void LogSettings::setWritingLogs(bool enabled)
{
	mShouldLogIntoConsole = enabled;
	mShouldLogIntoFile = enabled;
}

void LogSettings::setWritingEachLog()
{
	setWritingLogsFromEachModule();
	setWritingLogsFromEachLogType();
}

void LogSettings::setWritingLogsFromEachModule()
{
	setWritingLogsOnlyFromCertainModules(
		{ ModuleID::Base, ModuleID::Inputs, ModuleID::Logs, ModuleID::Music, ModuleID::Physics,
		ModuleID::Render, ModuleID::Resources, ModuleID::Sound, ModuleID::States }
	);
}

void LogSettings::setWritingLogsFromEachLogType()
{
	setWritingLogsOnlyFromCertainTypes( { LogType::ERROR, LogType::GOOD, LogType::WARNING } );
}

bool LogSettings::shouldThisLogBeWrittenIntoConsole(const Log& log)
{
	return mShouldLogIntoConsole && shouldThisLogBeWritten(log);
}

bool LogSettings::shouldThisLogBeWrittenIntoFile(const Log& log)
{
	return mShouldLogIntoFile && shouldThisLogBeWritten(log);
}

bool LogSettings::shouldThisLogBeWritten(const Log& log)
{
	return true;
}