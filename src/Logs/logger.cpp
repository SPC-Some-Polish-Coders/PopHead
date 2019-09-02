#include "logger.hpp"
#include "Utilities/filePath.hpp"

#include <chrono>
#include <ctime>
#include <string>
#include <algorithm>

namespace ph {

namespace {
	std::string numberToStringWithTwoDigits(unsigned int number)
	{
		if (number >= 10)
			return std::to_string(number);
		return '0' + std::to_string(number);
	}

	std::string getCurrentTimeAsString()
	{
		// TODO: change this function to use <chrono> instead of ctime and add miliseconds

		auto timePoint = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		tm calendarTime;
		localtime_s(&calendarTime, &timePoint);

		return numberToStringWithTwoDigits(calendarTime.tm_hour) + ":" +
			numberToStringWithTwoDigits(calendarTime.tm_min) + ":" +
			numberToStringWithTwoDigits(calendarTime.tm_sec);
	}
}

void Logger::createLog(LogLevel level, const std::string& message, const std::string& filePath, unsigned short fileLine)
{
	LogRecord logRecord;
	logRecord.level = level;
	logRecord.message = message;
	logRecord.filePath = FilePath::cutFilePathAndFixSeparator(filePath);
	logRecord.fileLine = fileLine;
	logRecord.secondsFromStart = getInstance().mClock.getElapsedTime().asSeconds();
	logRecord.time = getCurrentTimeAsString();

	auto& handlers = getInstance().mHandlers;

	for (auto& handler : handlers)
	{
		handler->handleLog(logRecord);
	}
}

void Logger::addLogsHandler(std::unique_ptr<Handler> handler)
{
	if (handler)
		getInstance().mHandlers.emplace_back(std::move(handler));
}

bool Logger::removeLogsHandler(const Handler& handler)
{
	auto& handlers = getInstance().mHandlers;
	auto iter = std::find_if(handlers.begin(), handlers.end(),
		[&handler](const std::unique_ptr<Handler>& elem) { return elem.get() == &handler; });
	
	if (iter != handlers.end())
	{
		handlers.erase(iter);
		return true;
	}
	return false;
}

Logger& Logger::getInstance()
{
	static Logger globalLogger;
	return globalLogger;
}

}