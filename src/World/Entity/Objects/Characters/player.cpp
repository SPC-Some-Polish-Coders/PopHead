#include "player.hpp"

using PopHead::World::Entity::Player;

Player::Player(Base::GameData* gameData)
:Character(gameData, "player")
{
}

void Player::input()
{

}

void Player::update(sf::Time delta)
{

}

/*void talkTo(NPC&)
{

}*/

/*auto getPerks() const -> const PerkManager&
{
    return mPerks;
}*/
