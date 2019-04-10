#include "input.hpp"
#include "Base/gameData.hpp"

using PopHead::Input::Input;

Input::Input()
:mAction()
{
}

void Input::setGameData(Base::GameData* gameData)
{
    gameData->getRenderer().getWindow().setKeyRepeatEnabled(false);

    mMouse.setGameData(gameData);
    mAction.setGameData(gameData);
    mKeyboard.setGameData(gameData);
}
