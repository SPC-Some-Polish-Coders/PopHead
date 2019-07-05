#pragma once

#include "actionManager.hpp"
#include "keyboardManager.hpp"
#include "mouseManager.hpp"
#include "globalKeyboardShortcuts.hpp"

namespace ph {

class GameData;

class Input
{
public:
	Input();

    auto getKeyboard() -> const KeyboardManager& { return mKeyboard; }
    auto getMouse() -> const MouseManager& { return mMouse; }
    auto getAction() -> ActionManager& { return mAction; }
	auto getGlobalKeyboardShortcutes() -> const GlobalKeyboardShortcuts& { return mGlobalKeyboardShortcuts; }

    void setGameData(GameData*);

private:
    MouseManager mMouse;
    ActionManager mAction;
    KeyboardManager mKeyboard;
	GlobalKeyboardShortcuts mGlobalKeyboardShortcuts;
};

}
