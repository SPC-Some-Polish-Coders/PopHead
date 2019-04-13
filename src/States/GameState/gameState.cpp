#include "States/GameState/gameState.hpp"

#include <iostream>

#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/map.hpp"

using PopHead::States::GameState;

GameState::GameState(PopHead::Base::GameData* const gameData)
:State{gameData}
{
    mPlayerTexture.loadFromFile("resources/textures/characters/vaultManSheet.png");
    std::unique_ptr<World::Entity::Player> player(new World::Entity::Player(gameData));
    player->getSprite().setTexture(mPlayerTexture);
    mRoot.addChild(std::move(player));

    mMapTexture.loadFromFile("resources/textures/map/city.png");
    std::unique_ptr<World::Entity::Map> city(new World::Entity::Map(gameData, "cityMap", mMapTexture, 2));
    mRoot.addChild(std::move(city));
}

void GameState::input()
{
    mRoot.input();
}

void GameState::update(sf::Time delta)
{
    mRoot.update(delta);
}
