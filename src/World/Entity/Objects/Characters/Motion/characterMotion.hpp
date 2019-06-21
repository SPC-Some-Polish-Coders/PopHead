#pragma once

namespace ph {

struct CharacterMotion
{
	CharacterMotion();
	void clear();
	bool isMoving();

	bool isMovingDiagonally();

	bool isMovingLeft;
	bool isMovingRight;
	bool isMovingUp;
	bool isMovingDown;
};

}
