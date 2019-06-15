#include "collisionDebugRect.hpp"

#include "Base/gameData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"

ph::CollisionDebugRect::CollisionDebugRect(ph::GameData* gameData, sf::FloatRect rect, ph::CollisionBody* owner)
	:Object(gameData, "collisionDebugRect", LayerID::collisionDebug)
	,mShape(sf::Vector2f(rect.width, rect.height))
	,mOwner(owner)
{
	mShape.setPosition(rect.left, rect.top);
}

void ph::CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	updateColor();

	if (shouldDisplay()) {
		target.draw(mShape, states);
	}
}

void ph::CollisionDebugRect::updateColor() const
{
	auto& settings = CollisionDebugSettings::getInstance();
	auto& newColor = settings.getFillColor(mOwner->getBodyType());
	mShape.setFillColor(newColor);
}

bool ph::CollisionDebugRect::shouldDisplay() const
{
	auto& settings = CollisionDebugSettings::getInstance();
	return settings.shouldDisplay(mOwner->getBodyType());
}
