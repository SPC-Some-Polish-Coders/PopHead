#include "collisionDebugRect.hpp"

#include "gameData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"

namespace ph {

CollisionDebugRect::CollisionDebugRect(sf::FloatRect rect)
	:mShape({rect.width, rect.height})
{
	mShape.setPosition(rect.left, rect.top);
}

void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	updateColor();
	if (shouldDisplay())
		target.draw(mShape, states);
}

void CollisionDebugRect::updateColor() const
{
	auto& settings = CollisionDebugSettings::getInstance();
	//auto& newColor = settings.getFillColor();
	auto newColor = sf::Color::Magenta;
	mShape.setFillColor(newColor);
}

bool CollisionDebugRect::shouldDisplay() const
{
	auto& settings = CollisionDebugSettings::getInstance();
	//return settings.shouldDisplay(mOwner->getBodyType());
	return true;
}

}