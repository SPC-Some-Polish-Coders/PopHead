#include "terminalHandler.hpp"
#include "Terminal/terminal.hpp"

#include <sstream>
#include <iomanip>

#include <SFML/Graphics/Color.hpp>

namespace ph {

	TerminalHandler::TerminalHandler(Terminal* terminal)
		: mTerminal(terminal)
	{
	}

	void TerminalHandler::utilizeLog(const LogRecord& logRecord)
	{
		std::string message = logRecordToString(logRecord);
		OutputLine line{ message, sf::Color::White };
		
		switch (logRecord.level)
		{
		case LogLevel::Info:
			line.mColor = sf::Color(127, 244, 44);
			break;
		case LogLevel::Error:
			line.mColor = sf::Color(255, 25, 33);
			break;
		case LogLevel::Warning:
			line.mColor = sf::Color(235, 135, 30);
			break;
		case LogLevel::Critical:
			line.mColor = sf::Color(255, 25, 33);
			break;
		}
		mTerminal->pushOutputLine(line);
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
