#ifndef POPHEAD_CMD_COMMANDPROMPT_H_
#define POPHEAD_CMD_COMMANDPROMPT_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
	namespace Base { class GameData; }
namespace Cmd {

class CommandPrompt : public sf::Drawable
{
public:
	CommandPrompt();

	void input();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setGameData(Base::GameData* gameData) { mGameData = gameData; }

private:
	Base::GameData* mGameData;

	sf::RectangleShape mCommandPromptBackground;
};

}}

#endif // !POPHEAD_CMD_COMMANDPROMPT_H_

