#include "characterMotion.hpp"

using PopHead::World::Entity::CharacterMotion;

CharacterMotion::CharacterMotion()
{
    clear();
}

void CharacterMotion::clear()
{
	isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
}

bool CharacterMotion::isMoving()
{
    return isMovingLeft || isMovingRight || isMovingUp || isMovingDown;
}
