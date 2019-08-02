#pragma once

#include <SFML/Window.hpp>

namespace ph{

class GameData;

class EventLoop
{
public:
    EventLoop() = delete;

    static void init(GameData*);

    static void eventLoop(GameData*);

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
    inline static sf::Keyboard::Key mKey;
    inline static sf::Mouse::Button mMouseButton;
    inline static bool mIsKeyJustPressed;
    inline static bool mIsKeyJustReleased;
    inline static bool mIsMouseButtonJustPressed;
    inline static bool mIsMouseButtonJustReleased;
    inline static bool mHasMouseJustMoved;
};

}
