#include "States/GameState/gameState.hpp"

#include <iostream>

#include "World/Entity/Objects/Characters/player.hpp"

using PopHead::States::GameState;

GameState::GameState(Base::GameData* const gameData)
:State{gameData}
{
    mTexture.loadFromFile("resources/textures/characters/vaultMan.png");

    std::unique_ptr<World::Entity::Player> player(new World::Entity::Player(gameData));
    player->getSprite().setTexture(mTexture);
    mRoot.addChild(std::move(player));
}

void GameState::input()
{
    mRoot.input();
}

void GameState::update(sf::Time delta)
{
    mRoot.update(delta);
}
