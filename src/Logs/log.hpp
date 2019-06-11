#ifndef POPHEAD_LOGS_LOG_H_
#define POPHEAD_LOGS_LOG_H_

#include <string>

namespace ph {
namespace Logs {

enum class LogType : std::size_t {
	Info,
	Error,
	Warning,
	Count // WARNING: Keep it last
};

struct LogData
{
	std::string message;
	std::string moduleName;
	LogType type;
};

}}

#endif // !POPHEAD_LOG_LOG_H_