#ifndef POPHEAD_LOGS_LOGMANAGER_H_
#define POPHEAD_LOGS_LOGMANAGER_H_

#include "LogData/moduleID.hpp"
#include "logSettings.hpp"
#include <SFML/System.hpp>
#include <vector>
#include <fstream>

namespace PopHead {
namespace Logs {


class LogManager
{
private:
	LogManager();
	LogManager(LogManager&) = delete;
	void operator=(LogManager&) = delete;

public:
	static LogManager& getLogManager()
	{
		static LogManager logManager; 
		return logManager;
	}

	auto getLogSettings() -> LogSettings& { return mLogSettings; }

	void writeLog(const Log& log);

private:
	void nameTheFile();
	void saveLogsInFile(const Log& log); 
	void writeLogInConsole(const Log& log);
	sf::Time getTimeFromStartOfTheProgram();

private:
	LogSettings mLogSettings;
	std::ofstream mLogFile;
	sf::Clock mTimeFromStartOfTheProgram;

public:
	friend std::ostream& operator<<(std::ostream& os, const ModuleID& dt);
	friend std::ostream& operator<<(std::ostream& os, const LogType& dt);
};


}}

#endif // !POPHEAD_LOGS_LOGMANAGER_H_