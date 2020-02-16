#pragma once

#include "Logs/handler.hpp"

namespace ph {

	class TerminalHandler : public Handler
	{
		virtual void utilizeLog(const LogRecord& logRecord);
		static std::string logRecordToString(const LogRecord& logRecord);
	};
}
