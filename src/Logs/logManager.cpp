#include "logManager.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

using PopHead::Logs::LogManager;
using PopHead::Logs::Log;
using PopHead::Logs::ModuleID;
using PopHead::Logs::LogType;


LogManager::LogManager()
{
	std::time_t t = time(0);
	struct tm now;
	//localtime_s(&now, &t);
	std::string fileName;
	//if (now.tm_mday < 10 && now.tm_mon < 10) fileName += "0" + std::to_string(now.tm_mon + 1) + "." + "0" + std::to_string(now.tm_mday);
	//if (now.tm_mday < 10 && now.tm_mon > 9) fileName += "0" + std::to_string(now.tm_mon + 1) + "." + std::to_string(now.tm_mday);
	//if (now.tm_mon < 10 && now.tm_mon > 9) fileName += std::to_string(now.tm_mon + 1) + "." + "0" + std::to_string(now.tm_mday);
	//if (now.tm_min < 10) fileName += "_" + std::to_string(now.tm_hour) + "-" + "0" + std::to_string(now.tm_min) + "-" + std::to_string(now.tm_sec);
	//else fileName += "_" + std::to_string(now.tm_hour) + "-" + std::to_string(now.tm_min) + "-" + std::to_string(now.tm_sec);

	mLogFile.open("logs/log_" + fileName + ".txt", std::ofstream::out | std::ofstream::app);
}

std::ostream& PopHead::Logs::operator<<(std::ostream& os, const LogType& dt)
{
	switch (dt) {
	case LogType::INFO:
		os << "INFO";
		break;
	case LogType::ERROR:
		os << "ERROR";
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
	case ModuleID::Renderer:
		os << "RENDERER";
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
	case ModuleID::None:
		os << "         ";
		break;
	}
	return os;
}

void LogManager::writeLog(const Log& log)
{
	if(mLogSettings.shouldThisLogBeWrittenIntoConsole(log))
		writeLogInConsole(log);

	if(mLogSettings.shouldThisLogBeWrittenIntoFile(log))
		saveLogsInFile(log);
}

void LogManager::writeLogInConsole(const Log& log)
{
	std::cout << log.type << " | " << log.moduleID << " | " << log.message << " | "
	<< std::setprecision(1) << "[" << getTimeFromStartOfTheProgram().asSeconds() << "s]" << std::endl;
}

void LogManager::saveLogsInFile(const Log& log)
{
	mLogFile << log.type << " | " << log.moduleID << " | " << log.message << " | "
	<< std::setprecision(1) << "[" << getTimeFromStartOfTheProgram().asSeconds() << "s]" << std::endl;
	mLogFile.flush();
}


sf::Time LogManager::getTimeFromStartOfTheProgram()
{
	sf::Time elapsed = mTimeFromStartOfTheProgram.getElapsedTime();
	return elapsed;
}
