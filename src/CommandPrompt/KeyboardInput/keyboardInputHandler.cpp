#include "keyboardInputHandler.hpp"

#include "Base/gameData.hpp"

using PopHead::CommandPrompt::KeyboardInputHandler;
using PopHead::CommandPrompt::CommandPromptSharedData;

KeyboardInputHandler::KeyboardInputHandler(CommandPromptSharedData commandPromptSharedData)
	:mCommandPromptSharedData(commandPromptSharedData)
	,mContent(mCommandPromptSharedData->mContent)
{
}

void KeyboardInputHandler::handleInput()
{
	handleKeyboardCharactersInput();
	handleBackspace();
	handleEnter();
	showOrHideCommandPromptInput();

	mCommandPromptSharedData->mText.setString(mCommandPromptSharedData->mContent);
}

void KeyboardInputHandler::handleKeyboardCharactersInput()
{
	auto& keyboard = mGameData->getInput().getKeyboard();

	if(keyboard.isKeyJustPressed(sf::Keyboard::A)) mContent += "a";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::B)) mContent += "b";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::C)) mContent += "c";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::D)) mContent += "d";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::E)) mContent += "e";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::F)) mContent += "f";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::G)) mContent += "g";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::H)) mContent += "h";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::I)) mContent += "i";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::J)) mContent += "j";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::K)) mContent += "k";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::L)) mContent += "l";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::M)) mContent += "m";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::N)) mContent += "n";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::O)) mContent += "o";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::P)) mContent += "p";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::Q)) mContent += "q";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::R)) mContent += "r";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::S)) mContent += "s";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::T)) mContent += "t";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::U)) mContent += "u";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::V)) mContent += "v";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::W)) mContent += "w";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::X)) mContent += "x";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::Y)) mContent += "y";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::Z)) mContent += "z";
	else if(keyboard.isKeyJustPressed(sf::Keyboard::Space)) mContent += " ";
}

void KeyboardInputHandler::handleBackspace()
{
	auto& keyboard = mGameData->getInput().getKeyboard();
	if(keyboard.isKeyJustPressed(sf::Keyboard::BackSpace)) {
		if(mContent.size() > 0)
			mContent.pop_back();
	}
}

void KeyboardInputHandler::handleEnter()
{
	auto& keyboard = mGameData->getInput().getKeyboard();
	if(keyboard.isKeyJustPressed(sf::Keyboard::Enter))
		mContent.clear();
}

void KeyboardInputHandler::showOrHideCommandPromptInput()
{
	auto& keyboard = mGameData->getInput().getKeyboard();
	if(keyboard.isKeyJustPressed(sf::Keyboard::Tab)) {
		bool& isVisible = mCommandPromptSharedData->mIsVisible;
		auto& actionManager = mGameData->getInput().getAction();
		actionManager.setEnabled(isVisible);
		isVisible = !isVisible;
	}
}
