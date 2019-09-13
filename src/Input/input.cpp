#include "input.hpp"
#include "gameData.hpp"
#include "eventLoop.hpp"

namespace ph {

Input::Input()
	:mAction()
{
}

void Input::setGameData(GameData* gameData)
{
	mGlobalKeyboardShortcuts.setGameData(gameData);
}

}
