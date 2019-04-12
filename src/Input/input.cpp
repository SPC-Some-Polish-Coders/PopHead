#include "input.hpp"
#include "Base/gameData.hpp"
#include "eventLoop.hpp"

using PopHead::Input::Input;


Input::Input()
:mAction()
{
}

void Input::setGameData(Base::GameData* gameData)
{
    mMouse.setGameData(gameData);
}
