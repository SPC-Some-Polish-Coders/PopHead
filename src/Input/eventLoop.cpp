#include "eventLoop.hpp"

#include "Base/gameData.hpp"
#include <SFML/Window.hpp>

using PopHead::Input::EventLoop;


void EventLoop::eventLoop()
{
    sf::Event event;

    while(mGameData->getRenderer().getWindow().pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
            break;

        case sf::Event::KeyReleased:
            break;

        case sf::Event::MouseButtonPressed:
            break;

        case sf::Event::MouseButtonReleased:
            break;

        case sf::Event::MouseMoved:
            break;
        }
    }
}
