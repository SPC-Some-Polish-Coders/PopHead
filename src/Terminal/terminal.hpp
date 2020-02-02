#pragma once

#include <string>
#include <memory>
#include "terminalSharedData.hpp"
#include "terminalInputHandler.hpp"
#include "commandInterpreter.hpp"
#include "terminalRenderer.hpp"
#include "Events/event.hpp"
#include <entt/entt.hpp>
#include <SFML/Window/Window.hpp>

namespace ph {

class Terminal
{
public:
	Terminal(sf::Window&);
	void init(SceneManager*);

	void handleEvent(Event&);
	void update(float dt);
	void pushOutputLine(const OutputLine&);

	auto getSharedData() -> TerminalSharedData & { return mTerminalSharedData; }

private:
	TerminalSharedData mTerminalSharedData;
	TerminalRenderer mTerminalRenderer;
	TerminalInputHandler mKeyboardInputHandler;
	CommandInterpreter mCommandInterpreter;
};

}
