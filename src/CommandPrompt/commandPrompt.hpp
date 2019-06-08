#ifndef POPHEAD_CMD_COMMANDPROMPT_H_
#define POPHEAD_CMD_COMMANDPROMPT_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "keyboardInputHandler.hpp"

namespace PopHead {
	namespace Base { class GameData; }
namespace CommandPrompt {

class CommandPrompt : public sf::Drawable
{
public:
	CommandPrompt();

	void input();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void move(sf::Vector2f offset);
	void init(Base::GameData*);
private:
	void initializeText();

private:
	sf::RectangleShape mCommandPromptBackground;
	std::shared_ptr<sf::Text> mText;
	std::shared_ptr<std::string> mContent;
	KeyboardInputHandler mKeyboardInputHandler;
	Base::GameData* mGameData;
	bool mIsVisible;
};

}}

#endif // !POPHEAD_CMD_COMMANDPROMPT_H_

