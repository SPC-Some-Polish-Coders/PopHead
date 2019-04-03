#include "States/GameState/gameState.hpp"

#include <iostream>

using PopHead::States::GameState;

GameState::GameState(Base::GameData* const gameData)
:State{gameData}
{
    //mRoot.addChild();
}

void GameState::input()
{

}

void GameState::update(sf::Time delta)
{
    std::cout<<"GAME STATE"<<std::endl;
}
