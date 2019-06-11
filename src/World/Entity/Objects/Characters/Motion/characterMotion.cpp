#include "characterMotion.hpp"

ph::CharacterMotion::CharacterMotion()
{
	clear();
}

void ph::CharacterMotion::clear()
{
	isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
}

bool ph::CharacterMotion::isMoving()
{
	return isMovingLeft || isMovingRight || isMovingUp || isMovingDown;
}

bool ph::CharacterMotion::isMovingDiagonally()
{
	return (isMovingLeft || isMovingRight) && (isMovingUp || isMovingDown);
}
