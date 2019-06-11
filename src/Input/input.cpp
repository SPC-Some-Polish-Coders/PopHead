#include "input.hpp"
#include "Base/gameData.hpp"
#include "eventLoop.hpp"

using ph::Input::Input;

Input::Input()
	:mAction()
{
}

void Input::setGameData(ph::Base::GameData* gameData)
{
	mMouse.setGameData(gameData);
}
