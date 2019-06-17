#ifndef POPHEAD_TERMINAL_TERMINALSHAREDDATA_H_
#define POPHEAD_TERMINAL_TERMINALSHAREDDATA_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

namespace ph {

struct TerminalData
{
	sf::Text mInputLine;
	std::string mContent;
	std::string mLastCommand;
	bool mIsVisible = false;
};

using TerminalSharedData = std::shared_ptr<TerminalData>;

}

#endif // !POPHEAD_TERMINAL_TERMINALSHAREDDATA_H_

