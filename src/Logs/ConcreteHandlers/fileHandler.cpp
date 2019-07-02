#include "fileHandler.hpp"

#include <ctime>
#include <iomanip>

ph::FileHandler::FileHandler(const std::string& baseFileName)
{
	// TODO: change this function to use <chrono> instead of ctime

	std::time_t t = time(0);
	std::tm* now = localtime(&t);
	std::string fileName = baseFileName;
	fileName += std::to_string(now->tm_mon + 1) + "." + std::to_string(now->tm_mday) +
		"_" + std::to_string(now->tm_hour) + "-" + std::to_string(now->tm_min) +
		"-" + std::to_string(now->tm_sec) + ".txt";

	mLogFile.open(fileName);
}

void ph::FileHandler::utilizeLog(const LogRecord& logRecord)
{
	mLogFile << "[  " << std::left << std::setw(7)
			 << std::to_string(logRecord.secondsFromStart).erase(5, 4) << "s ]"
			 << " | " << std::setw(9) << std::left << logTypeToString(logRecord.type)
			 << " | " << std::setw(9) << std::left << logRecord.moduleName
			 << " | " << std::left << logRecord.message << std::endl;
}
