#include "collisionDebugManager.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

void CollisionDebugManager::addStaticBodyCollisionDebugRect(const CollisionBody& staticBody)
{
	mStaticBodyCollisionDebugRects.emplace_back(staticBody.getRect());
}

void CollisionDebugManager::addKinematicBodyCollisionDebugRect(const CollisionBody& kinematicBody)
{
	mKinematicBodyCollisionDebugRects.emplace_back(kinematicBody.getRect());
}

void CollisionDebugManager::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	for(const auto& debugRect : mStaticBodyCollisionDebugRects)
		debugRect.draw(target, states);

	for(const auto& debugRect : mKinematicBodyCollisionDebugRects)
		debugRect.draw(target, states);
}

}