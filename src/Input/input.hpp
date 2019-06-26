#pragma once

#include "Input/actionManager.hpp"
#include "Input/keyboardManager.hpp"
#include "Input/mouseManager.hpp"

namespace ph {

class GameData;

class Input
{
public:
    Input();

    auto getKeyboard() -> const KeyboardManager& {return mKeyboard;}
    auto getMouse()    -> const MouseManager&    {return mMouse;}
    auto getAction()   -> ActionManager&         {return mAction;}

    void setGameData(GameData*);

private:
    MouseManager mMouse;
    ActionManager mAction;
    KeyboardManager mKeyboard;
};

}
