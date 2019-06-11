#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERMOTION_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERMOTION_H_

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

#endif // !POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERMOTION_H_
