#include "collisionDebugRect.hpp"

using PopHead::World::Entity::CollisionDebugRect;

void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mRect, states);
}
