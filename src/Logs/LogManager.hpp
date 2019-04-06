#ifndef POPHEAD_LOGS_LOGMANAGER_H_
#define POPHEAD_LOGS_LOGMANAGER_H_

namespace PopHead{
namespace Logs {

class LogManager{
pubilc:
    LogManager ();
    writeLog(log : Log) : void
    stopWritingLogsInConsole() : void
    startWritingLogsInConsole() : void
    writeLogsOnlyFromCertainModules(std::vector<ModuleID>) : void
    writeLogsFromEachModule() : void
    writeLogsOnlyFromCertainLogTypes(std::vector<LogType>) : void
    writeLogsFromEachLogType() : void
    writeEachLog() : void
    getTimeFromStartOfTheProgram()const :sf::Time&


private:
    gatheredLogs: std::vector<Log>
    timeFromStartOfTheProgram : sf::Clock
    LogFromModulesToWrite : std::vector<ModuleID>
    TypesOfLogToWrite : srd:vector<LogType>
    stop : bool
    saveLogsInFile() : void

};


}}

#endif // POPHEAD_LOGS_LOGMANAGER_H_
