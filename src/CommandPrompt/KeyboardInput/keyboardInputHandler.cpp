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
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::A))
		mContent += "a";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::B))
		mContent += "b";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::C))
		mContent += "c";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::D))
		mContent += "d";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::E))
		mContent += "e";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F))
		mContent += "f";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::G))
		mContent += "g";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::H))
		mContent += "h";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::I))
		mContent += "i";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::J))
		mContent += "j";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::K))
		mContent += "k";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::L))
		mContent += "l";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::M))
		mContent += "m";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::N))
		mContent += "n";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::O))
		mContent += "o";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::P))
		mContent += "p";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Q))
		mContent += "q";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::R))
		mContent += "r";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::S))
		mContent += "s";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::T))
		mContent += "t";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::U))
		mContent += "u";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::V))
		mContent += "v";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::W))
		mContent += "w";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::X))
		mContent += "x";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Y))
		mContent += "y";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Z))
		mContent += "z";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Space))
		mContent += " ";
}

void KeyboardInputHandler::handleBackspace()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::BackSpace)) {
		if(mContent.size() > 0)
			mContent.pop_back();
	}
}

void KeyboardInputHandler::handleEnter()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Enter))
		mContent.clear();
}

void KeyboardInputHandler::showOrHideCommandPromptInput()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Tab)) {
		bool& isVisible = mCommandPromptSharedData->mIsVisible;
		mGameData->getInput().getAction().setEnabled(isVisible);
		isVisible = !isVisible;
	}
}
