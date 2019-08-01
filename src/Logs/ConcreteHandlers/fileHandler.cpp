#include "fileHandler.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>

namespace {

	std::string numberToStringWithTwoDigits(unsigned int number)
	{
		if (number >= 10)
			return std::to_string(number);
		return '0' + std::to_string(number);
	}
}

ph::FileHandler::FileHandler(std::string fileName)
{
	// TODO: change this function to use <chrono> instead of ctime

	auto timePoint = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm calendarTime;
	localtime_s(&calendarTime, &timePoint);

	fileName += '_' +
				numberToStringWithTwoDigits(calendarTime.tm_mon + 1) + '-' +
				numberToStringWithTwoDigits(calendarTime.tm_mday) + '_' +
				numberToStringWithTwoDigits(calendarTime.tm_hour) + '-' +
				numberToStringWithTwoDigits(calendarTime.tm_min) + '-' +
				numberToStringWithTwoDigits(calendarTime.tm_sec) + ".txt";

	mLogFile.open(fileName);
}

void ph::FileHandler::utilizeLog(const LogRecord& logRecord)
{
	mLogFile << "[  " << std::left << std::setw(7)
			 << std::to_string(logRecord.secondsFromStart).erase(5, 4) << "s ]"
			 << " | " << std::setw(9) << std::left << logLevelToString(logRecord.level)
			 << " | " << std::setw(9) << std::left << logRecord.moduleName
			 << " | " << std::left << logRecord.message << std::endl;
}
