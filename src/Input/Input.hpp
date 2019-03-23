#ifndef POPHEAD_INPUT_INPUT_H_
#define POPHEAD_INPUT_INPUT_H_

#include "actionManager.hpp"
#include "keyboardManager.hpp"
#include "mouseManager.hpp"

namespace PopHead {
namespace Input {


class Input
{
public:
    auto getKeyboard() -> const KeyboardManager&;
    auto getMouse()    -> const MouseManager&;
    auto getAction()   -> const ActionManager&;

private:
    ActionManager mAction;
    KeyboardManager mKeyboard;
    MouseManager mMouse;
};


}}

#endif // !POPHEAD_INPUT_INPUT_H_
