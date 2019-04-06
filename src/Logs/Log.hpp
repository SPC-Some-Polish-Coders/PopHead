#ifndef POPHEAD_LOGS_LOG_H_
#define POPHEAD_LOGS_LOG_H_

namespace PopHead{
namespace Logs{

struct Log{
    Log ();
    LogType type;
    ModuleID moduleID;
    std::string message;
};


}}

#endif // POPHEAD_LOGS_LOG_H_
