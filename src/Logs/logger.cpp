#include "logger.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

using PopHead::Logs::Logger;
using PopHead::Logs::LogData;
using PopHead::Logs::LogType;

Logger::Logger()
{
	openFile();
}

std::ostream& operator<<(std::ostream& os, const LogType& logType)
{
	switch (logType) {
	case LogType::Info:
		os << "INFO";
		break;
	case LogType::Error:
		os << "ERROR";
		break;
	case LogType::Warning:
		os << "WARNING";
		break;
	}
	return os;
}

std::string Logger::nameTheFile()
{
	std::time_t t = time(0);
	std::tm* now = localtime(&t);
	std::string fileName;

	if (now->tm_mday < 10 && now->tm_mon < 10) fileName += "0" + std::to_string(now->tm_mon + 1) + "." + "0" + std::to_string(now->tm_mday);
	else if (now->tm_mday < 10 && now->tm_mon > 9) fileName += "0" + std::to_string(now->tm_mon + 1) + "." + std::to_string(now->tm_mday);
	else if (now->tm_mon < 10 && now->tm_mon > 9) fileName += std::to_string(now->tm_mon + 1) + "." + "0" + std::to_string(now->tm_mday);
	else fileName += std::to_string(now->tm_mon + 1) + "." + std::to_string(now->tm_mday);

	if (now->tm_min < 10) fileName += "_" + std::to_string(now->tm_hour) + "-" + "0" + std::to_string(now->tm_min) + "-" + std::to_string(now->tm_sec);
	else fileName += "_" + std::to_string(now->tm_hour) + "-" + std::to_string(now->tm_min) + "-" + std::to_string(now->tm_sec);

	return fileName;
}

void Logger::openFile()
{
	mLogFile.open("logs/log_" + nameTheFile() + ".txt", std::ofstream::out | std::ofstream::app);
	if (!mLogFile.is_open())
		PH_EXCEPTION("Log file could not be created. Make sure that 'logs' folder exists in your game directory.");
}

void Logger::writeLog(const LogData& log)
{
	if (mLogSettings.shouldBeWrittenIntoConsole(log))
		writeLogInConsole(log);

	if (mLogSettings.shouldBeWrittenIntoFile(log))
		saveLogsInFile(log);
}

void Logger::writeLogInConsole(const LogData& log)

{
	std::cout  << "[  " << std::left << std::setw(7) << std::to_string(getElapsedTimeSinceCreation().asSeconds()).erase(5, 4) << "s ]"
		<< " | " << std::setw(7) << std::left << log.type
		<< " | " << std::setw(9) << std::left << log.moduleName
		<< " | " << std::left << log.message << std::endl;
}

void Logger::saveLogsInFile(const LogData& log)

{
	mLogFile << "[  " << std::left << std::setw(7) << std::to_string(getElapsedTimeSinceCreation().asSeconds()).erase(5, 4) << "s ]"
		<< " | " << std::setw(7) << std::left << log.type
		<< " | " << std::setw(9) << std::left << log.moduleName
		<< " | " << std::left << log.message << std::endl
		<< std::flush;
}

sf::Time Logger::getElapsedTimeSinceCreation()
{
	const sf::Time elapsedTime = mClock.getElapsedTime();
	return elapsedTime;
}
