#include "eventLoop.hpp"

#include "Base/gameData.hpp"
#include <SFML/Window.hpp>

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
            isKeyPressed = true;
            break;

        case sf::Event::KeyReleased:
            isKeyReleased = true;
            break;

        case sf::Event::MouseButtonPressed:
            isMouseButtonPressed = true;
            break;

        case sf::Event::MouseButtonReleased:
            isMouseButtonReleased = true;
            break;

        case sf::Event::MouseMoved:
            hasMouseMoved = true;
            break;
        }
    }
}

void EventLoop::clear()
{
    isKeyPressed = false;
    isKeyReleased = false;
    isMouseButtonPressed = false;
    isMouseButtonReleased = false;
    hasMouseMoved = false;
}
