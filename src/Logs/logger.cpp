#include "pch.hpp"
#include "logger.hpp"
#include "Utilities/filePath.hpp"

namespace ph {

namespace {
	std::string numberToStringWithTwoDigits(u32 number)
	{
		if (number >= 10)
			return std::to_string(number);
		return '0' + std::to_string(number);
	}

	std::string getCurrentTimeAsString()
	{
		auto timepoint = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		tm calendarTime;
		localtime_s(&calendarTime, &timepoint);

		return numberToStringWithTwoDigits(calendarTime.tm_hour) + ":" +
			numberToStringWithTwoDigits(calendarTime.tm_min) + ":" +
			numberToStringWithTwoDigits(calendarTime.tm_sec);
	}
}

void Logger::createLog(LogLevel level, const std::string& message, const std::string& filePath, u32 fileLine)
{
	LogRecord logRecord;
	logRecord.level = level;
	logRecord.message = message;
	logRecord.filePath = FilePath::cutFilePathAndFixSeparator(filePath);
	logRecord.fileLine = fileLine;
	logRecord.secondsFromStart = getInstance().mClock.getElapsedTime().asSeconds();
	logRecord.time = getCurrentTimeAsString();

	auto& mutex = getInstance().mHandlersMutex;
	std::lock_guard lock(mutex);

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
