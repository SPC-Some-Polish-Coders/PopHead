#include "logger.hpp"
#include "gameData.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

namespace ph {

static std::ostream& operator<<(std::ostream& os, const LogType& logType)
{
	switch(logType) {
	case LogType::Info:
		os << "INFO";
		break;
	case LogType::Error:
		os << "ERROR";
		break;
	case LogType::Warning:
		os << "WARNING";
		break;
	case LogType::FromUser:
		os << "FROM USER";
		break;
	case LogType::Exception:
		os << "EXCEPTION";
		break;
	case LogType::UnhandledException:
		os << "UNHANDLED EXCEPTION";
		break;
	}
	return os;
}

Logger::Logger()
	:mGameData(nullptr)
{
	openFile();
}

std::string Logger::nameTheFile()
{
	std::time_t t = time(0);
	std::tm* now = localtime(&t);
	std::string fileName;
	return fileName += std::to_string(now->tm_mon+1) + "." + std::to_string(now->tm_mday) + 
		"_" + std::to_string(now->tm_hour) + "-" + std::to_string(now->tm_min)+
		"-" + std::to_string(now->tm_sec);
}

void Logger::openFile()
{
	mLogFile.open("logs/log_" + nameTheFile() + ".txt", std::ofstream::out | std::ofstream::app);
	if (!mLogFile.is_open()) {
		// WARNING: Don't use PH_EXCEPTION or PH_LOG here if this method is called in Logger constructor becouse it can result in recursion
		std::cout << "[Logger::openFile] Log file could not be created. Make sure that 'logs' folder exists in your game directory" << std::endl;
		throw std::runtime_error("Log file could not be created. Make sure that 'logs' folder exists in your game directory");
	}
}

void Logger::writeLog(const LogData& log)
{
	if (mLogSettings.shouldBeWrittenIntoConsole(log))
		writeLogInConsole(log);

	if (mLogSettings.shouldBeWrittenIntoTerminal(log))
		writeLogInInternalTerminal(log);

	if (mLogSettings.shouldBeWrittenIntoFile(log))
		saveLogInFile(log);
}

void Logger::writeLogInConsole(const LogData& log)
{
	std::cout << printLog(log).str();
}

void Logger::writeLogInInternalTerminal(const LogData& log)
{
	if(mGameData != nullptr) {
		std::string message = printLog(log).str();
		OutputLine line{ message, sf::Color::White };

		switch (log.type)
		{
		case LogType::Info:
			line.mColor = sf::Color(127, 244, 44);
			break;
		case LogType::Error:
			line.mColor = sf::Color(255, 25, 33);
			break;
		case LogType::Warning:
			line.mColor = sf::Color(235, 135, 30);
			break;
		case LogType::FromUser:
			line.mColor = sf::Color(79, 202, 255);
			break;
		case LogType::Exception:
			line.mColor = sf::Color(255, 25, 33);
			break;
		case LogType::UnhandledException:
			line.mColor = sf::Color(255, 25, 33);
			break;
		}
		mGameData->getTerminal().pushOutputLine(line);
	}
}

void Logger::saveLogInFile(const LogData& log)
{
	mLogFile << printLog(log).str();
}

std::stringstream Logger::printLog(const LogData& log)
{
	std::stringstream ssLog;
	ssLog << "[  " << std::left << std::setw(7)
		  << std::to_string(getElapsedTimeSinceCreation().asSeconds()).erase(5, 4) << "s ]"
		  << " | " << std::setw(9) << std::left << log.type
		  << " | " << std::setw(9) << std::left << log.moduleName
		  << " | " << std::left << log.message << std::endl;
	return ssLog;
}

sf::Time Logger::getElapsedTimeSinceCreation()
{
	const sf::Time elapsedTime = mClock.getElapsedTime();
	return elapsedTime;
}

}