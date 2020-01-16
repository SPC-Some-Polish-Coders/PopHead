#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "terminalSharedData.hpp"
#include "terminalInputHandler.hpp"
#include "commandInterpreter.hpp"
#include "terminalRenderer.hpp"
#include "Events/event.hpp"
#include <entt/entt.hpp>

namespace ph {

class GameData;

class Terminal
{
public:
	Terminal();
	void init(GameData*);

	void setSceneRegistry(entt::registry*);
	void handleEvent(const ph::Event&);
	void update();
	void pushOutputLine(const OutputLine&);

	auto getSharedData() -> TerminalSharedData & { return mTerminalSharedData; }

private:
	TerminalSharedData mTerminalSharedData;
	TerminalRenderer mTerminalRenderer;
	TerminalInputHandler mKeyboardInputHandler;
	CommandInterpreter mCommandInterpreter;
	GameData* mGameData;
};

}
