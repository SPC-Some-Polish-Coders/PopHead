#ifndef POPHEAD_LOGS_LOG_H_
#define POPHEAD_LOGS_LOG_H_
#include "Logs/ModuleID.hpp"
#include "Logs/LogType.hpp"
#include <string>

namespace PopHead{
namespace Logs{

struct Log{
    Log(LogType, ModuleID, const std::string& message);
    LogType type;
    ModuleID moduleID;
    const std::string& message;
};


}}

#endif // POPHEAD_LOGS_LOG_H_
