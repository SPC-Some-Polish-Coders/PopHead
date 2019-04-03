#include "States/GameState/gameState.hpp"

#include <iostream>

#include "World/Entity/Objects/Characters/player.hpp"

using PopHead::States::GameState;

GameState::GameState(Base::GameData* const gameData)
:State{gameData}
{
    std::unique_ptr<World::Entity::Player> player(new World::Entity::Player(gameData));
    mRoot.addChild(std::move(player));
}

void GameState::input()
{

}

void GameState::update(sf::Time delta)
{
    std::cout<<"GAME STATE"<<std::endl;
}
