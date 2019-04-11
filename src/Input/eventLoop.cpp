#include "eventLoop.hpp"

#include "Base/gameData.hpp"

using PopHead::Input::EventLoop;

bool EventLoop::mIsKeyPressed;
bool EventLoop::mIsKeyReleased;
bool EventLoop::mIsMouseButtonPressed;
bool EventLoop::mIsMouseButtonReleased;
bool EventLoop::mHasMouseMoved;
sf::Keyboard::Key EventLoop::mKey;
sf::Mouse::Button EventLoop::mMouseButton;

void EventLoop::init(Base::GameData* gameData)
{
    gameData->getRenderer().getWindow().setKeyRepeatEnabled(false);
}

void EventLoop::eventLoop(Base::GameData* gameData)
{
    clear();
    sf::Event event;

    while(gameData->getRenderer().getWindow().pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
            mIsKeyPressed = true;
            mKey = event.key.code;
            break;

        case sf::Event::KeyReleased:
            mIsKeyReleased = true;
            mKey = event.key.code;
            break;

        case sf::Event::MouseButtonPressed:
            mIsMouseButtonPressed = true;
            mMouseButton = event.mouseButton.button;
            break;

        case sf::Event::MouseButtonReleased:
            mIsMouseButtonReleased = true;
            mMouseButton = event.mouseButton.button;
            break;

        case sf::Event::MouseMoved:
            mHasMouseMoved = true;
            ///TODO mouse position
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
