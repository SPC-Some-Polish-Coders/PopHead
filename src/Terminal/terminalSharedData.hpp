#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <deque>

namespace ph {

struct TerminalData
{
	std::string content;
	std::deque<std::string> lastCommands;
	bool isVisible = false;
};

using TerminalSharedData = std::shared_ptr<TerminalData>;

}
