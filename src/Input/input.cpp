#include "Input/input.hpp"

using PopHead::Input::Input;
using PopHead::Input::ActionManager;
using PopHead::Input::KeyboardManager;
using PopHead::Input::MouseManager;

auto Input::getKeyboard() -> const KeyboardManager&
{
	return mKeyboard;
}

auto Input::getMouse() -> const MouseManager&
{
	return mMouse;
}

auto Input::getAction()   -> const ActionManager&
{
	return mAction;
}
