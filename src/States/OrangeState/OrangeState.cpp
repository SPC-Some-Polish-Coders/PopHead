#include "OrangeState.hpp"

#include <iostream>

using PopHead::States::OrangeState;

OrangeState::OrangeState(Base::GameData *gameData)
:gameData(gameData)
{
}

void OrangeState::input()
{

}

void OrangeState::update(sf::Time delta)
{
    std::cout<<"ORANGE"<<std::endl;
}
