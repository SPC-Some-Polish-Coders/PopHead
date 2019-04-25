#include "collisionBody.hpp"

using PopHead::Physics::CollisionBody;

CollisionBody::CollisionBody(sf::FloatRect rect, float mass, PopHead::Base::GameData* gameData)
:mRect(rect)
,mMass(mass)
{
	
}

void CollisionBody::move(sf::Vector2f velocity)
{
	mVelocity.x += velocity.x;
	mVelocity.y += velocity.y;
}
