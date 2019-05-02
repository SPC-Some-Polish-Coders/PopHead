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
public:
	LogManager(bool shouldWritingLogs);
	void writeLog(Log log);
	void stopWritingLogsInConsole();
	void startWritingLogsInConsole();
	void writeLogsOnlyFromCertainModules(std::vector<ModuleID> moduleID);
	void writeLogsFromEachModule();
	void writeLogsOnlyFromCertainLogTypes(std::vector<LogType> logType);
	void writeEachLog();
	auto getTimeFromStartOfTheProgram() const->sf::Time &; 
	friend std::ostream& operator<<(std::ostream& os, const ModuleID& dt);
	friend std::ostream& operator<<(std::ostream& os, const LogType& dt);
private:
	std::ofstream logFile;
	std::vector<Log> gatheredLogs;
	sf::Clock timeFromStartOfTheProgram;
	std::vector<ModuleID> logFromModulesToWrite;
	std::vector<LogType> typesOfLogToWrite;
	bool stop; 
	void saveLogsInFile();
};

}}

#endif // !POPHEAD_LOGS_LOGMANAGER_H_