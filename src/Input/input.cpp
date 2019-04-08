#include "input.hpp"
#include "Base/gameData.hpp"

using PopHead::Input::Input;

void Input::setGameData(Base::GameData* gameData)
{
    gameData->getRenderer().getWindow().setKeyRepeatEnabled(false);

    mKeyboard.setGameData(gameData);
}
