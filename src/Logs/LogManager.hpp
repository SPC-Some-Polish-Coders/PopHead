#ifndef POPHEAD_LOGS_LOGMANAGER_H_
#define POPHEAD_LOGS_LOGMANAGER_H_

namespace PopHead{
namespace Logs {

class LogManager{
pubilc:
    LogManager ();
    void writeLog(log : Log)
    void stopWritingLogsInConsole()
    void startWritingLogsInConsole()
    void writeLogsOnlyFromCertainModules(std::vector<ModuleID>)
    void writeLogsFromEachModule()
    void writeLogsOnlyFromCertainLogTypes(std::vector<LogType>)
    void writeLogsFromEachLogType()
    void writeEachLog()
    sf::Time& getTimeFromStartOfTheProgram()const


private:
    std::vector<Log> gatheredLogs
    sf::Clock timeFromStartOfTheProgram
    std::vector<ModuleID>  LogFromModulesToWrite
    std:vector<LogType> TypesOfLogToWrite
    bool stop
    void saveLogsInFile()

};


}}

#endif // POPHEAD_LOGS_LOGMANAGER_H_
