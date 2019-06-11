#ifndef POPHEAD_TERMINAL_COMMANDPROMPT_H_
#define POPHEAD_TERMINAL_COMMANDPROMPT_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "terminalSharedData.hpp"
#include "Input/terminalInputHandler.hpp"
#include "Commands/commandInterpreter.hpp"

namespace ph {

class GameData;

class Terminal : public sf::Drawable
{
public:
	Terminal();

	void input();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void move(sf::Vector2f offset);
	void init(GameData*);
private:
	void initializeText();

private:
	TerminalSharedData mTerminalSharedData;
	sf::RectangleShape mTerminalBackground;
	TerminalInputHandler mKeyboardInputHandler;
	CommandInterpreter mCommandInterpreter;
	GameData* mGameData;
};

}

#endif // !POPHEAD_CMD_COMMANDPROMPT_H_
