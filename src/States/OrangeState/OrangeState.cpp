#include "OrangeState.hpp"

#include <iostream>
#include "States/StateIdentifiers.hpp"

using PopHead::States::OrangeState;

OrangeState::OrangeState(Base::GameData *gameData)
:gameData(gameData)
{
}

void OrangeState::input()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        std::cout<<"Q"<<std::endl;
    }
}

void OrangeState::update(sf::Time delta)
{
    std::cout<<"ORANGE"<<std::endl;
}
