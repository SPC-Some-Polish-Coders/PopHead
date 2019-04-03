#include "characterMotion.hpp"

using PopHead::World::Entity::CharacterMotion;

CharacterMotion::CharacterMotion()
{
    clear();
}

void CharacterMotion::clear()
{
    isMovingLeft  = false;
    isMovingRight = false;
    isMovingUp    = false;
    isMovingDown  = false;
}
