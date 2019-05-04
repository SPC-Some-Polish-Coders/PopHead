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

LogManager::LogManager(bool stopWritingLogs)
	: stop(stopWritingLogs)
{
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	std::string fileName;
	if (now.tm_mday < 10 && now.tm_mon < 10) fileName += "0" + std::to_string(now.tm_mon + 1) + "." + "0" + std::to_string(now.tm_mday);
	if (now.tm_mday < 10 && now.tm_mon > 9) fileName += "0" + std::to_string(now.tm_mon + 1) + "." + std::to_string(now.tm_mday);
	if (now.tm_mon < 10 && now.tm_mon > 9) fileName += std::to_string(now.tm_mon + 1) + "." + "0" + std::to_string(now.tm_mday);

	if (now.tm_min < 10) fileName += "_" + std::to_string(now.tm_hour) + "-" + "0" + std::to_string(now.tm_min);
	else fileName += "_" + std::to_string(now.tm_hour) + "-" + std::to_string(now.tm_min);
	logFile.open("savedLogs/log_" + fileName + ".txt", std::ofstream::out | std::ofstream::app);
	if (stopWritingLogs) { stopWritingLogsInConsole(); }
}


void LogManager::writeLog(Log log)
{
	gatheredLogs.push_back(log);
	if (stop == false) startWritingLogsInConsole();
	saveLogsInFile();
}

void LogManager::stopWritingLogsInConsole()
{
}

void LogManager::startWritingLogsInConsole()
{
	writeEachLog();
}

void LogManager::writeLogsOnlyFromCertainModules(std::vector <ModuleID> moduleID)
{
}

void LogManager::writeLogsFromEachModule()
{
}

void LogManager::writeLogsOnlyFromCertainLogTypes(std::vector <LogType> logType)
{
}

void LogManager::writeEachLog()
{
	std::cout << gatheredLogs.back().type << " | " << gatheredLogs.back().moduleID << " | '" << gatheredLogs.back().message << "' | "
		<< std::setprecision(1) << "[" << getTimeFromStartOfTheProgram().asSeconds() << "s]" << std::endl;
}

auto LogManager::getTimeFromStartOfTheProgram() const->sf::Time &
{
	sf::Time elapsed = timeFromStartOfTheProgram.getElapsedTime();
	return elapsed;
}

void LogManager::saveLogsInFile()
{
	logFile << gatheredLogs.back().type << " | " << gatheredLogs.back().moduleID << " | '" << gatheredLogs.back().message << "' | "
		<< std::setprecision(1) << "[" << getTimeFromStartOfTheProgram().asSeconds() << "s]" << std::endl;
	logFile.flush();
}
