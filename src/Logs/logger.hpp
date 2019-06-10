#ifndef POPHEAD_LOGS_LOGGER_H_
#define POPHEAD_LOGS_LOGGER_H_

#include "log.hpp"
#include "logSettings.hpp"
#include "Utilities/path.hpp"
#include <SFML/System.hpp>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>

using PopHead::Logs::LogData;
using PopHead::Logs::LogType;

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
	void saveLogInFile(const LogData& log); 
	void writeLogInConsole(const LogData& log);
	std::stringstream printLog(const LogData& log);
	std::string nameTheFile();
	sf::Time getElapsedTimeSinceCreation();

private:
	LogSettings mLogSettings;
	std::ofstream mLogFile;
	sf::Clock mClock;
};


}}

#endif // !POPHEAD_LOGS_LOGGER_H_