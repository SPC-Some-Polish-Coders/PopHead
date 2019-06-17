#include "logger.hpp"
#include "Base/gameData.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

static std::ostream& operator<<(std::ostream& os, const ph::LogType& logType)
{
	switch(logType) {
	case ph::LogType::Info:
		os << "INFO";
		break;
	case ph::LogType::Error:
		os << "ERROR";
		break;
	case ph::LogType::Warning:
		os << "WARNING";
		break;
	case ph::LogType::FromUser:
		os << "FROM USER";
		break;
	}
	return os;
}

ph::Logger::Logger()
	:mGameData(nullptr)
{
	openFile();
}

std::string ph::Logger::nameTheFile()
{
	std::time_t t = time(0);
	std::tm* now = localtime(&t);
	std::string fileName;
	return fileName += std::to_string(now->tm_mon+1) + "." + std::to_string(now->tm_mday) + 
		"_" + std::to_string(now->tm_hour) + "-" + std::to_string(now->tm_min)+
		"-" + std::to_string(now->tm_sec);
}

void ph::Logger::openFile()
{
	mLogFile.open("logs/log_" + nameTheFile() + ".txt", std::ofstream::out | std::ofstream::app);
	if (!mLogFile.is_open()) {
		// WARNING: Don't use PH_EXCEPTION or PH_LOG here if this method is called in Logger constructor becouse it can result in recursion
		std::cout << "[Logger::openFile] Log file could not be created. Make sure that 'logs' folder exists in your game directory" << std::endl;
		throw std::runtime_error("Log file could not be created. Make sure that 'logs' folder exists in your game directory");
	}
}

void ph::Logger::writeLog(const LogData& log)
{
	if (mLogSettings.shouldBeWrittenIntoConsole(log))
		writeLogInConsole(log);

	writeLogInInternalTerminal(log);

	if (mLogSettings.shouldBeWrittenIntoFile(log))
		saveLogInFile(log);
}

void ph::Logger::writeLogInConsole(const LogData& log)
{
	std::cout << printLog(log).str();
}

void ph::Logger::writeLogInInternalTerminal(const LogData& log)
{
	if(mGameData != nullptr) {
		std::string message = printLog(log).str();
		OutputLine line{ message, sf::Color::White };
		if (log.type == LogType::Info)
			line.mColor = sf::Color(127, 244, 44);
		else if (log.type == LogType::Error)
			line.mColor = sf::Color(255, 25, 33);
		else if (log.type == LogType::Warning)
			line.mColor = sf::Color(235, 135, 30);
		else if (log.type == LogType::FromUser)
			line.mColor = sf::Color(79, 202, 255);
		mGameData->getTerminal().pushOutputLine(line);
	}
}

void ph::Logger::saveLogInFile(const LogData& log)
{
	mLogFile << printLog(log).str();
}

std::stringstream ph::Logger::printLog(const LogData& log)
{
	std::stringstream ssLog;
	ssLog << "[  " << std::left << std::setw(7)
		  << std::to_string(getElapsedTimeSinceCreation().asSeconds()).erase(5, 4) << "s ]"
		  << " | " << std::setw(9) << std::left << log.type
		  << " | " << std::setw(9) << std::left << log.moduleName
		  << " | " << std::left << log.message << std::endl;
	return ssLog;
}

sf::Time ph::Logger::getElapsedTimeSinceCreation()
{
	const sf::Time elapsedTime = mClock.getElapsedTime();
	return elapsedTime;
}

