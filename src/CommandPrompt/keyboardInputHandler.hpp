#ifndef POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_
#define POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

namespace PopHead {
	namespace Base { class GameData; }
namespace CommandPrompt {

class KeyboardInputHandler
{
public:
	KeyboardInputHandler(std::shared_ptr<sf::Text> text, std::shared_ptr<std::string> content);
	
	void setGameData(Base::GameData* gameData) { mGameData = gameData; }

	void handleInput();

private:
	Base::GameData* mGameData;
	std::shared_ptr<sf::Text> mText;
	std::shared_ptr<std::string> mContent;
};

}}

#endif // !POPHEAD_COMMANDPROMPT_KEYBOARDINPUTHANDLER_H_

