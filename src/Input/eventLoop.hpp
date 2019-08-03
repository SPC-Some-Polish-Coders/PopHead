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

	static auto getPressedKey() -> sf::Keyboard::Key { return mPressedKey; }
    static auto getReleasedKey() -> sf::Keyboard::Key { return mReleasedKey; }
    static auto getMouseButton() -> sf::Mouse::Button { return mMouseButton; }
    static bool isKeyJustPressed(){ return mIsKeyJustPressed; }
    static bool isKeyJustReleased(){ return mIsKeyJustReleased; }
    static bool isMouseButtonJustPressed(){ return mIsMouseButtonJustPressed; }
    static bool isMouseButtonJustReleased(){ return mIsMouseButtonJustReleased; }
    static bool hasMouseJustMoved(){ return mHasMouseJustMoved; }

private:
    static void clear();

private:
	inline static sf::Keyboard::Key mPressedKey;
    inline static sf::Keyboard::Key mReleasedKey;
    inline static sf::Mouse::Button mMouseButton;
    inline static bool mIsKeyJustPressed;
    inline static bool mIsKeyJustReleased;
    inline static bool mIsMouseButtonJustPressed;
    inline static bool mIsMouseButtonJustReleased;
    inline static bool mHasMouseJustMoved;
};

}
