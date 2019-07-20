#pragma once

#include "Logs/handler.hpp"

namespace ph {

	class Terminal;

	class TerminalHandler : public Handler
	{
	public:
		TerminalHandler(Terminal* terminal);

	private:
		virtual void utilizeLog(const LogRecord& logRecord);
		static std::string logRecordToString(const LogRecord& logRecord);

		Terminal* mTerminal;
	};
}
