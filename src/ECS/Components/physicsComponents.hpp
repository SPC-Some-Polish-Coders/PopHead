#pragma once


namespace ph::component {

	struct Position
	{
		float x, y;
	};

	struct Size
	{
		float width, height;
	};

	struct Velocity
	{
		float dx, dy;
	};

	struct CharacterSpeed
	{
		float speed;
	};

	struct StaticCollisionBody
	{
	};
	
	struct KinematicCollisionBody 
	{
		float mass;
	};
}
