#ifndef POPHEAD_LOGS_LOGGER_H_
#define POPHEAD_LOGS_LOGGER_H_

#include "LogData/logType.hpp"
#include "logSettings.hpp"
#include <SFML/System.hpp>
#include <vector>
#include <fstream>

using PopHead::Logs::Log;
using PopHead::Logs::LogType;

#define PH_LOG(logType, message) PopHead::Logs::Logger::getLogger().writeLog(PopHead::Logs::Log(logType, __FILE__, message))

namespace PopHead {
namespace Logs {

class Logger
{
private:
	Logger();

public:
	Logger(Logger&) = delete;
	void operator=(Logger&) = delete;

	static Logger& getLogger()
	{
		static Logger Logger; 
		return Logger;
	}

	auto getLogSettings() -> LogSettings& { return mLogSettings; }

	void writeLog(const Log& log);

private:
	void openFile();
	void saveLogsInFile(const Log& log); 
	void writeLogInConsole(const Log& log);
	sf::Time getElapsedTimeSinceCreation();

private:
	LogSettings mLogSettings;
	std::ofstream mLogFile;
	sf::Clock mClock;

public:
	friend std::ostream& operator<<(std::ostream& os, const LogType& logType);
};


}}

#endif // !POPHEAD_LOGS_LOGGER_H_