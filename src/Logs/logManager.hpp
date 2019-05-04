#ifndef POPHEAD_LOGS_LOGMANAGER_H_
#define POPHEAD_LOGS_LOGMANAGER_H_

#include "log.hpp"
#include "moduleID.hpp"
#include <SFML/System.hpp>
#include <vector>
#include <fstream>

namespace PopHead {
namespace Logs {


class LogManager
{
private:
	LogManager();
	//LogManager(LogManager&) = delete;
	//operator=(LogManager&) = delete;

public:
	static LogManager& getLogManager()
	{
		static LogManager logManager; 
		return logManager;
	}

	void writeLog(Log log);
	void stopWritingLogsInConsole();
	void startWritingLogsInConsole();
	void writeLogsOnlyFromCertainModules(std::vector<ModuleID> moduleID);
	void writeLogsFromEachModule();
	void writeLogsOnlyFromCertainLogTypes(std::vector<LogType> logType);
	void writeEachLog();
	sf::Time& getTimeFromStartOfTheProgram();
	
private:
	 void saveLogsInFile(); 

private:
	 std::ofstream logFile;
	 std::vector<Log> gatheredLogs;
	 sf::Clock timeFromStartOfTheProgram;
	 std::vector<ModuleID> logFromModulesToWrite;
	 std::vector<LogType> typesOfLogToWrite;

public:
	friend std::ostream& operator<<(std::ostream& os, const ModuleID& dt);
	friend std::ostream& operator<<(std::ostream& os, const LogType& dt);
};


}}

#endif // !POPHEAD_LOGS_LOGMANAGER_H_