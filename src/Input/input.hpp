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

    auto getAction() -> ActionManager& { return mAction; }
	auto getGlobalKeyboardShortcuts() -> GlobalKeyboardShortcuts& { return mGlobalKeyboardShortcuts; }

    void setGameData(GameData*);

private:
    ActionManager mAction;
	GlobalKeyboardShortcuts mGlobalKeyboardShortcuts;
};

}
