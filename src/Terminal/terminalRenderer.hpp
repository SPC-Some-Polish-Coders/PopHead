#pragma once

#include "Terminal/terminalSharedData.hpp"

namespace ph {

struct OutputLine
{
	std::string mText;
	sf::Color mColor;
};

class TerminalRenderer
{
public:
	TerminalRenderer(TerminalSharedData);

	void pushOutputLine(const OutputLine& line);
	void update() const;

private:
	std::deque<OutputLine> mOutputLines;
	TerminalSharedData mTerminalSharedData;
};

}

