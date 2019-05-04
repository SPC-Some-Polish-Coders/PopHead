#include "logSettings.hpp"

using PopHead::Logs::LogSettings;
using PopHead::Logs::ModuleID;
using PopHead::Logs::LogType;

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


bool LogSettings::shouldThisLogBeWrittenIntoConsole()
{
	return true;
}

bool LogSettings::shouldThisLogBeWrittenIntoFile()
{
	return true;
}