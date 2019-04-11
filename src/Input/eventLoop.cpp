#include "eventLoop.hpp"

#include "Base/gameData.hpp"

using PopHead::Input::EventLoop;


void EventLoop::eventLoop()
{
    clear();
    sf::Event event;

    while(mGameData->getRenderer().getWindow().pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
            mIsKeyPressed = true;
            break;

        case sf::Event::KeyReleased:
            mIsKeyReleased = true;
            break;

        case sf::Event::MouseButtonPressed:
            mIsMouseButtonPressed = true;
            break;

        case sf::Event::MouseButtonReleased:
            mIsMouseButtonReleased = true;
            break;

        case sf::Event::MouseMoved:
            mHasMouseMoved = true;
            break;
        }
    }
}

void EventLoop::clear()
{
    mIsKeyPressed = false;
    mIsKeyReleased = false;
    mIsMouseButtonPressed = false;
    mIsMouseButtonReleased = false;
    mHasMouseMoved = false;
    mKey = sf::Keyboard::Unknown;
    mMouseButton = sf::Mouse::ButtonCount;
}
