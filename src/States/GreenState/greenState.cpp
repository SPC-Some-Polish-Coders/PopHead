#include "greenState.hpp"

#include <iostream>

using PopHead::States::GreenState;

GreenState::GreenState(Base::GameData* const gameData)
:State{gameData}
{
}

void GreenState::input()
{

}

void GreenState::update(sf::Time delta)
{
    std::cout<<"GREEN"<<std::endl;

}
