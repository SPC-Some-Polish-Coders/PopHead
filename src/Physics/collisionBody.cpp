#include "collisionBody.hpp"

using PopHead::Physics::CollisionBody;

CollisionBody::CollisionBody(sf::FloatRect rect, float mass)
:mRect(rect)
,mMass(mass)
{
}

void CollisionBody::addToForce(sf::Vector2f force)
{
    mForce.x += force.x;
    mForce.y += force.y;
}
