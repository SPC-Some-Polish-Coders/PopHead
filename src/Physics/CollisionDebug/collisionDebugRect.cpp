#include "collisionDebugRect.hpp"

#include "gameData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"

namespace ph {

CollisionDebugRect::CollisionDebugRect(GameData* gameData, sf::FloatRect rect, CollisionBody* owner)
	:Object(gameData, "collisionDebugRect", LayerID::collisionDebug)
	,mShape(sf::Vector2f(rect.width, rect.height))
	,mOwner(owner)
{
	mShape.setPosition(rect.left, rect.top);
}

void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	updateColor();

	if (shouldDisplay()) {
		target.draw(mShape, states);
	}
}

void CollisionDebugRect::updateColor() const
{
	auto& settings = CollisionDebugSettings::getInstance();
	auto& newColor = settings.getFillColor(mOwner->getBodyType());
	mShape.setFillColor(newColor);
}

bool CollisionDebugRect::shouldDisplay() const
{
	auto& settings = CollisionDebugSettings::getInstance();
	return settings.shouldDisplay(mOwner->getBodyType());
}

}