#include "logManager.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

using PopHead::Logs::LogManager;
using PopHead::Logs::Log;
using PopHead::Logs::ModuleID;
using PopHead::Logs::LogType;

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

LogManager::LogManager(bool shouldWritingLogs)
	: stop(shouldWritingLogs), typesOfLogToWrite(3), logFromModulesToWrite(10)
{
	typesOfLogToWrite = { LogType::GOOD, LogType::WARNING, LogType::FATAL };
	logFromModulesToWrite = { ModuleID::Base, ModuleID::Logs, ModuleID::Music, ModuleID::Sound, ModuleID::World,
		ModuleID::Render, ModuleID::Physics, ModuleID::States, ModuleID::Inputs, ModuleID::Resources };
	if (!shouldWritingLogs) { stopWritingLogsInConsole(); }
}


void LogManager::writeLog(Log log)
{
	gatheredLogs.push_back(log);
	logFromModulesToWrite.push_back(log.moduleID);
	typesOfLogToWrite.push_back(log.type);
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
	std::cout << gatheredLogs.back().type << " | " << gatheredLogs.back().moduleID << " | " << gatheredLogs.back().message << " | "
		<< std::setprecision(1) << getTimeFromStartOfTheProgram().asSeconds() << "s" << std::endl;
}

auto LogManager::getTimeFromStartOfTheProgram() const->sf::Time &
{
	sf::Time elapsed = timeFromStartOfTheProgram.getElapsedTime();
	return elapsed;
}

void LogManager::saveLogsInFile()
{
	std::ofstream logFile;
	logFile.open("Log.txt", std::ios::app);
	logFile << gatheredLogs.back().type << " | " << gatheredLogs.back().moduleID << " | '" << gatheredLogs.back().message << "' | "
		<< std::setprecision(1) << getTimeFromStartOfTheProgram().asSeconds() << "s" << std::endl;
	logFile.flush();
	logFile.close();
}
