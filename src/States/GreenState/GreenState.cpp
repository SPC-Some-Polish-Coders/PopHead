#include "GreenState.hpp"

#include <iostream>

using PopHead::States::GreenState;

GreenState::GreenState(Base::GameData *gameData)
:gameData(gameData)
{
}

void GreenState::input()
{

}

void GreenState::update(sf::Time delta)
{
    std::cout<<"GREEN"<<std::endl;
}
