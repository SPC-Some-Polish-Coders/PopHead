#include "keyboardInputHandler.hpp"

#include "Base/gameData.hpp"

using PopHead::CommandPrompt::KeyboardInputHandler;
using PopHead::CommandPrompt::CommandPromptSharedData;

KeyboardInputHandler::KeyboardInputHandler(CommandPromptSharedData commandPromptSharedData)
	:mCommandPromptSharedData(commandPromptSharedData)
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
	auto& content = mCommandPromptSharedData->mContent;

	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::A))
		content += "a";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::B))
		content += "b";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::C))
		content += "c";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::D))
		content += "d";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::E))
		content += "e";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F))
		content += "f";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::G))
		content += "g";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::H))
		content += "h";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::I))
		content += "i";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::J))
		content += "j";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::K))
		content += "k";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::L))
		content += "l";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::M))
		content += "m";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::N))
		content += "n";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::O))
		content += "o";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::P))
		content += "p";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Q))
		content += "q";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::R))
		content += "r";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::S))
		content += "s";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::T))
		content += "t";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::U))
		content += "u";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::V))
		content += "v";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::W))
		content += "w";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::X))
		content += "x";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Y))
		content += "y";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Z))
		content += "z";
	else if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Space))
		content += " ";
}

void KeyboardInputHandler::handleBackspace()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::BackSpace)) {
		auto& content = mCommandPromptSharedData->mContent;
		if(content.size() > 0)
			content.pop_back();
	}
}

void KeyboardInputHandler::handleEnter()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Enter))
		mCommandPromptSharedData->mContent.clear();
}

void KeyboardInputHandler::showOrHideCommandPromptInput()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::Tab)) {
		auto& isVisible = mCommandPromptSharedData->mIsVisible;
		mGameData->getInput().getAction().setEnabled(isVisible);
		isVisible = !isVisible;
	}
}
