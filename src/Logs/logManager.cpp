#include "logManager.hpp"

using PopHead::Logs::LogManager;
using PopHead::Logs::Log;
using PopHead::Logs::ModuleID;
using PopHead::Logs::LogType;

LogManager::LogManager(bool shouldWritingLogs)
	: stop(shouldWritingLogs)
{
	if (shouldWritingLogs == true) { startWritingLogsInConsole(); }
}

void LogManager::writeLog(Log log)
{
}

void LogManager::stopWritingLogsInConsole()
{
}

void LogManager::startWritingLogsInConsole()
{

}

void LogManager::writeLogsOnlyFromCertainModules(std::vector<ModuleID> moduleID)
{
}

void LogManager::writeLogsFromEachModule()
{
}

void LogManager::writeLogsOnlyFromCertainLogTypes(std::vector<LogType> logType)
{
}

void LogManager::writeEachLog()
{
}

/*auto LogManager::getTimeFromStartOfTheProgram() const->sf::Time &
{

}*/

void LogManager::saveLogsInFile()
{
}