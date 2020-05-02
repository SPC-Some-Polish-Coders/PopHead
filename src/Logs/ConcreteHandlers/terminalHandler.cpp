#include "pch.hpp"
#include "terminalHandler.hpp"
#include "Terminal/terminal.hpp"

namespace ph {

	void TerminalHandler::utilizeLog(const LogRecord& logRecord)
	{
		OutputLine line{ logRecordToString(logRecord), sf::Color::White };
		
		switch (logRecord.level)
		{
		case LogLevel::Info:
			line.color = sf::Color(127, 244, 44);
			break;
		case LogLevel::Error:
			line.color = sf::Color(255, 25, 33);
			break;
		case LogLevel::Warning:
			line.color = sf::Color(235, 135, 30);
			break;
		case LogLevel::Critical:
			line.color = sf::Color(255, 25, 33);
			break;
		}
		Terminal::pushOutputLine(line);
	}

	std::string TerminalHandler::logRecordToString(const LogRecord& logRecord)
	{
		std::ostringstream stream;
		stream << "[  " << std::left << std::setw(7)
			   << std::to_string(logRecord.secondsFromStart).erase(5, 4) << "s ]"
			   << " | " << std::setw(9) << std::left << logLevelToString(logRecord.level)
			   << " | " << std::setw(9) << std::left << logRecord.filePath
			   << " | " << std::left << logRecord.message << std::endl;

		return stream.str();
	}
}
