#ifndef POPHEAD_INPUT_EVENTLOOP_H_
#define POPHEAD_INPUT_EVENTLOOP_H_

#include <SFML/Window.hpp>

namespace ph{
    namespace Base{ class GameData; }
namespace Input{


class EventLoop
{
public:
    EventLoop() = delete;

    static void init(Base::GameData*);

    static void eventLoop(Base::GameData*);

    static bool isKeyJustPressed(){return mIsKeyJustPressed;}
    static bool isKeyJustReleased(){return mIsKeyJustReleased;}
    static bool isMouseButtonJustPressed(){return mIsMouseButtonJustPressed;}
    static bool isMouseButtonJustReleased(){return mIsMouseButtonJustReleased;}
    static bool hasMouseJustMoved(){return mHasMouseJustMoved;}
    static auto getKey() -> sf::Keyboard::Key {return mKey;}
    static auto getMouseButton() -> sf::Mouse::Button {return mMouseButton;}

private:
    static void clear();

private:
    static bool mIsKeyJustPressed;
    static bool mIsKeyJustReleased;
    static bool mIsMouseButtonJustPressed;
    static bool mIsMouseButtonJustReleased;
    static bool mHasMouseJustMoved;

    static sf::Keyboard::Key mKey;
    static sf::Mouse::Button mMouseButton;
};


}}

#endif // !POPHEAD_INPUT_EVENTLOOP_H_
