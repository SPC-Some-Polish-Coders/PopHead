#include "States/OrangeState/orangeState.hpp"

#include <iostream>
#include "States/stateIdentifiers.hpp"

using PopHead::States::OrangeState;

OrangeState::OrangeState(Base::GameData* const gameData)
:State{gameData}
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
    mGameData->getStateMachine().replaceState(StateID::GreenState);
}
