#ifndef POPHEAD_LOGS_LOGMANAGER_H_
#define POPHEAD_LOGS_LOGMANAGER_H_

#include "LogData/log.hpp"
#include "LogData/moduleID.hpp"
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

	void writeLog(const Log& log);
	
private:
	void saveLogsInFile(const Log& log); 
	void writeLogInConsole(const Log& log);

public:
	void stopWritingLogsInConsole();
	void writeLogsOnlyFromCertainModules(std::vector<ModuleID> moduleID);
	void writeLogsFromEachModule();
	void writeLogsOnlyFromCertainLogTypes(std::vector<LogType> logType);
	void writeEachLog();
	sf::Time getTimeFromStartOfTheProgram();

private:
	 std::ofstream mLogFile;
	 sf::Clock mTimeFromStartOfTheProgram;
	 std::vector<ModuleID> mLogFromModulesToWrite;
	 std::vector<LogType> mTypesOfLogToWrite;

public:
	friend std::ostream& operator<<(std::ostream& os, const ModuleID& dt);
	friend std::ostream& operator<<(std::ostream& os, const LogType& dt);
};


}}

#endif // !POPHEAD_LOGS_LOGMANAGER_H_