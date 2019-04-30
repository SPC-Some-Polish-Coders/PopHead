#include "logManager.hpp"
#include <iostream>

using PopHead::Logs::LogManager;
using PopHead::Logs::Log;
using PopHead::Logs::ModuleID;
using PopHead::Logs::LogType;

LogManager::LogManager(bool shouldWritingLogs)
	: stop(shouldWritingLogs)
{
	if (shouldWritingLogs == true) { startWritingLogsInConsole(); }
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

auto LogManager::getTimeFromStartOfTheProgram() const->sf::Time &
{
	sf::Time elapsed = timeFromStartOfTheProgram.getElapsedTime();
	return elapsed;
}

void LogManager::saveLogsInFile()
{
}

std::ostream& PopHead::Logs::operator<<(std::ostream& os, const LogType& dt)
{
	switch (dt) {
	case LogType::GOOD:
		os << "GOOD";
		break;
	case LogType::FATAL:
		os << "FATAL";
		break;
	case LogType::WARNING:
		os << "WARNING";
		break;
	}
	return os;
}

std::ostream& PopHead::Logs::operator<<(std::ostream& os, const ModuleID& dt)
{
	switch (dt)
	{
	case ModuleID::Base:
		os << "BASE";
		break;
	case ModuleID::Logs:
		os << "LOGS";
		break;
	case ModuleID::Music:
		os << "MUSIC";
		break;
	case ModuleID::Sound:
		os << "SOUND";
		break;
	case ModuleID::World:
		os << "WORLD";
		break;
	case ModuleID::Render:
		os << "RENDER";
		break;
	case ModuleID::Physics:
		os << "PHYSICS";
		break;
	case ModuleID::States:
		os << "STATES";
		break;
	case ModuleID::Inputs:
		os << "INPUTS";
		break;
	case ModuleID::Resources:
		os << "RESOURCES";
		break;
	}
	return os;
}

void LogManager::writeLog(Log log)
{
	std::cout << log.type << " | " << log.moduleID << " | " << log.message << " | " << std::endl << " | "
		<< getTimeFromStartOfTheProgram().asSeconds;
	gatheredLogs.push_back(log);
	logFromModulesToWrite.push_back(log.moduleID);
	typesOfLogToWrite.push_back(log.type);
}