#pragma once

#include "EntityComponentSystem/object.hpp"

namespace ph {

enum class ShootDirection 
{
	east, northEast, north, northWest, west, southWest, south, southEast
};

class Bullet
{
public:
	Bullet(const float damage, const float range);

private:
	void dealDamage();

private:
	const float mDamage;
	const float mRange;
};

class Gun : public Object
{
public:
	Gun(GameData* const, const float damage);

	void shoot(const ShootDirection);

private:
	const float mDamage;
};

}