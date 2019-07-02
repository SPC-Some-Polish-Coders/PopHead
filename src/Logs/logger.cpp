#include "logger.hpp"
#include "Utilities/path.hpp"

namespace ph {

void Logger::createLog(LogType type, const std::string& message, const std::string& fileName, unsigned short fileLine)
{
	LogRecord logRecord;
	logRecord.type = type;
	logRecord.message = message;
	logRecord.fileName = fileName;
	logRecord.fileLine = fileLine;
	logRecord.moduleName = Path::toModuleName(fileName);
	logRecord.secondsFromStart = getInstance().mClock.getElapsedTime().asSeconds();

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