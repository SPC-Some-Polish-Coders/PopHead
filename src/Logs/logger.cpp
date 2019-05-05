#include "logger.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

using PopHead::Logs::Logger;
using PopHead::Logs::Log;
using PopHead::Logs::ModuleID;
using PopHead::Logs::LogType;


Logger::Logger()
{
	openTheFile();
}

std::ostream & PopHead::Logs::operator<<(std::ostream & os, const LogType & dt)
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

std::ostream& PopHead::Logs::operator<<(std::ostream & os, const ModuleID & dt)
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

<<<<<<< HEAD:src/Logs/logManager.cpp
void LogManager::openTheFile()
{
	std::string fileNameDate(__DATE__);
	std::string fileNameTime(__TIME__);
	fileNameDate.replace(3, 1, "_"); fileNameDate.replace(6, 1, "_");
	fileNameTime.replace(2, 1, "-"); fileNameTime.replace(5, 1, "-");

	mLogFile.open("logs/log_" + fileNameDate + "_" + fileNameTime + ".txt", std::ofstream::out | std::ofstream::app);
}

void LogManager::writeLog(const Log & log)
=======
void Logger::writeLog(const Log& log)
>>>>>>> master:src/Logs/logger.cpp
{
	if (mLogSettings.shouldThisLogBeWrittenIntoConsole(log))
		writeLogInConsole(log);

	if (mLogSettings.shouldThisLogBeWrittenIntoFile(log))
		saveLogsInFile(log);
}

<<<<<<< HEAD:src/Logs/logManager.cpp
void LogManager::writeLogInConsole(const Log & log)
=======
void Logger::writeLogInConsole(const Log& log)
>>>>>>> master:src/Logs/logger.cpp
{
	std::cout << std::setprecision(3) << "[  " << std::left << std::setw(7) << getTimeFromStartOfTheProgram().asSeconds() << "s ]"
		<< " | " << std::setw(7) << std::left << log.type
		<< " | " << std::setw(9) << std::left << log.moduleID
		<< " | " << std::left << log.message << std::endl;
}

<<<<<<< HEAD:src/Logs/logManager.cpp
void LogManager::saveLogsInFile(const Log & log)
=======
void Logger::saveLogsInFile(const Log& log)
>>>>>>> master:src/Logs/logger.cpp
{
	mLogFile << std::setprecision(3) << "[  " << std::left << std::setw(7) << getTimeFromStartOfTheProgram().asSeconds() << "s ]"
		<< " | " << std::setw(7) << std::left << log.type
		<< " | " << std::setw(9) << std::left << log.moduleID
		<< " | " << std::left << log.message << std::endl;
}


sf::Time Logger::getTimeFromStartOfTheProgram()
{
	sf::Time elapsed = mTimeFromStartOfTheProgram.getElapsedTime();
	return elapsed;
}
