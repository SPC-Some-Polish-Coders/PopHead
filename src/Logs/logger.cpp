#include "logger.hpp"
#include "Utilities/filePath.hpp"

#include <ctime>
#include <string>

namespace ph {

std::string getCurrentTimeAsString()
{
	// TODO: change this function to use <chrono> instead of ctime and add miliseconds

	auto timePoint = std::time(nullptr);
	auto calendarTime = std::localtime(&timePoint);
	
	return std::to_string(calendarTime->tm_hour) + ":" + 
		   std::to_string(calendarTime->tm_min) + ":" + 
		   std::to_string(calendarTime->tm_sec);
}

void Logger::createLog(LogLevel level, const std::string& message, const std::string& fileName, unsigned short fileLine)
{
	LogRecord logRecord;
	logRecord.level = level;
	logRecord.message = message;
	logRecord.fileName = FilePath::toFilename(fileName);
	logRecord.fileLine = fileLine;
	logRecord.moduleName = FilePath::toModuleName(fileName);
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

Logger& Logger::getInstance()
{
	static Logger globalLogger;
	return globalLogger;
}

}