#ifndef POPHEAD_INPUT_INPUT_H_
#define POPHEAD_INPUT_INPUT_H_

#include "Input/actionManager.hpp"
#include "Input/keyboardManager.hpp"
#include "Input/mouseManager.hpp"

namespace PopHead {
    namespace Base{ class GameData; }
namespace Input {


class Input
{
public:
    Input();

    auto getKeyboard() -> const KeyboardManager& {return mKeyboard;}
    auto getMouse()    -> const MouseManager&    {return mMouse;}
    auto getAction()   -> const ActionManager&   {return mAction;}

    void setGameData(Base::GameData*);

private:
    ActionManager mAction;
    KeyboardManager mKeyboard;
    MouseManager mMouse;
};


}}

#endif // !POPHEAD_INPUT_INPUT_H_
