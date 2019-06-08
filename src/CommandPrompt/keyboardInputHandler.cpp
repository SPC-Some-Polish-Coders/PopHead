#include "keyboardInputHandler.hpp"

#include "Base/gameData.hpp"

using PopHead::CommandPrompt::KeyboardInputHandler;

KeyboardInputHandler::KeyboardInputHandler(std::shared_ptr<sf::Text> text, std::shared_ptr<std::string> content)
	:mText(text)
	,mContent(content)
{
}

void KeyboardInputHandler::handleInput()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::A))
		* mContent += "a";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::B))
		* mContent += "b";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::C))
		* mContent += "c";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::D))
		* mContent += "d";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::E))
		* mContent += "e";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F))
		* mContent += "f";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::G))
		* mContent += "g";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::H))
		* mContent += "h";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::I))
		* mContent += "i";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::J))
		* mContent += "j";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::K))
		* mContent += "k";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::L))
		* mContent += "l";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::M))
		* mContent += "m";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::N))
		* mContent += "n";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::O))
		* mContent += "o";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::P))
		* mContent += "p";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Q))
		* mContent += "q";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::R))
		* mContent += "r";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::S))
		* mContent += "s";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::T))
		* mContent += "t";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::U))
		* mContent += "u";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::V))
		* mContent += "v";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::W))
		* mContent += "w";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::X))
		* mContent += "x";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Y))
		* mContent += "y";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Z))
		* mContent += "z";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Space))
		* mContent += " ";

	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::BackSpace))
		mContent->pop_back();
	
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Enter))
		mContent->clear();

	mText->setString(*mContent);
}
