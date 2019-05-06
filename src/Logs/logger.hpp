#ifndef POPHEAD_LOGS_LOGGER_H_
#define POPHEAD_LOGS_LOGGER_H_

#define PH_LOG(logType, message) PopHead::Logs::Logger::getLogger().writeLog(PopHead::Logs::Log(logType, __FILE__, message))

#include "LogData/logType.hpp"
#include "logSettings.hpp"
#include <SFML/System.hpp>
#include <vector>
#include <fstream>

using PopHead::Logs::Log;
using PopHead::Logs::LogType;

namespace PopHead {
namespace Logs {

class Logger
{
private:
	Logger();
	Logger(Logger&) = delete;
	void operator=(Logger&) = delete;

public:
	static Logger& getLogger()
	{
		static Logger Logger; 
		return Logger;
	}

	auto getLogSettings() -> LogSettings& { return mLogSettings; }

	void writeLog(const Log& log);

private:
	void openTheFile();
	void saveLogsInFile(const Log& log); 
	void writeLogInConsole(const Log& log);
	sf::Time getTimeFromStartOfTheProgram();

private:
	LogSettings mLogSettings;
	std::ofstream mLogFile;
	sf::Clock mTimeFromStartOfTheProgram;

public:
	friend std::ostream& operator<<(std::ostream& os, const LogType& dt);
};


}}

#endif // !POPHEAD_LOGS_LOGGER_H_