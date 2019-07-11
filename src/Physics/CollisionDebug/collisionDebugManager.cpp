#include "collisionDebugManager.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "collisionDebugSettings.hpp"
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
	auto& settings = CollisionDebugSettings::getInstance();

	if(settings.shouldDisplay(BodyType::staticBody)) {
		auto& color = settings.getFillColor(BodyType::staticBody);
		for(auto& debugRect : mStaticBodyCollisionDebugRects) {
			debugRect.setColor(color);
			debugRect.draw(target, states);
		}
	}

	if(settings.shouldDisplay(BodyType::kinematicBody)) {
		auto& color = settings.getFillColor(BodyType::kinematicBody);
		for(auto& debugRect : mKinematicBodyCollisionDebugRects) {
			debugRect.setColor(color);
			debugRect.draw(target, states);
		}
	}
}

}