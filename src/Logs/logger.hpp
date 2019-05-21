#ifndef POPHEAD_LOGS_LOGGER_H_
#define POPHEAD_LOGS_LOGGER_H_

#include "log.hpp"
#include "logSettings.hpp"
#include "Utilities/parser.hpp"
#include "assert.hpp"
#include <SFML/System.hpp>
#include <vector>
#include <fstream>
#include <stdexcept>

using PopHead::Logs::LogData;
using PopHead::Logs::LogType;

#define PH_LOG(logType, message)\
	PopHead::Logs::Logger::getLogger().writeLog(PopHead::Logs::LogData{message, PopHead::Utilities::Parser::toModuleName(std::string(__FILE__)), logType})

#define PH_EXCEPTION(message) (void)(PH_LOG(PopHead::Logs::LogType::Error, message), PH_BREAKPOINT(), throw std::runtime_error(message))

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

	void writeLog(const LogData& log);

private:
	void openFile();
	void saveLogsInFile(const LogData& log); 
	void writeLogInConsole(const LogData& log);
	std::string nameTheFile();
	sf::Time getElapsedTimeSinceCreation();

private:
	LogSettings mLogSettings;
	std::ofstream mLogFile;
	sf::Clock mClock;

	friend std::ostream& operator<<(std::ostream& os, const LogType& logType);
};


}}

#endif // !POPHEAD_LOGS_LOGGER_H_