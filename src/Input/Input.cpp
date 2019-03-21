#include "Input.hpp"

using PopHead::Input::Input;

auto Input::getKeyboard() -> const KeyboardManager&
{
    return mKeyboard;
}

auto Input::getMouse() -> const MouseManager&
{
    return mMouse;
}

auto Input::getAction()   -> const MouseManager&
{
    mAction;
}
