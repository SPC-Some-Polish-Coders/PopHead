#ifndef POPHEAD_INPUT_EVENTLOOP_H_
#define POPHEAD_INPUT_EVENTLOOP_H_

#include <SFML/Window.hpp>

namespace PopHead{
    namespace Base{ class GameData; }
namespace Input{


class EventLoop
{
public:
    ///TODO change eventLoop to operator()
    static void eventLoop(Base::GameData*);

    ///TODO change for example isKeyPressed to isKeyJustPressed
    static bool isKeyPressed(){return mIsKeyPressed;}
    static bool isKeyReleased(){return mIsKeyReleased;}
    static bool isMouseButtonPressed(){return mIsMouseButtonPressed;}
    static bool isMouseButtonReleased(){return mIsMouseButtonReleased;}
    static bool hasMouseMoved(){return mHasMouseMoved;}
    static auto getKey() -> sf::Keyboard::Key {return mKey;}
    static auto getMouseButton() -> sf::Mouse::Button {return mMouseButton;}

private:
    static void clear();

private:
    static bool mIsKeyPressed;
    static bool mIsKeyReleased;
    static bool mIsMouseButtonPressed;
    static bool mIsMouseButtonReleased;
    static bool mHasMouseMoved;

    static sf::Keyboard::Key mKey;
    static sf::Mouse::Button mMouseButton;
};


}}

#endif // !POPHEAD_INPUT_EVENTLOOP_H_
