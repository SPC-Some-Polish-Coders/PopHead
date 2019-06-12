#include "input.hpp"
#include "Base/gameData.hpp"
#include "eventLoop.hpp"

ph::Input::Input()
	:mAction()
{
}

void ph::Input::setGameData(GameData* gameData)
{
	mMouse.setGameData(gameData);
}
