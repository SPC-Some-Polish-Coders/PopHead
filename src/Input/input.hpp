#pragma once

#include "actionManager.hpp"

namespace ph {

class Input
{
public:
	Input();
    auto getAction() -> ActionManager& { return mAction; }

private:
    ActionManager mAction;
};

}
