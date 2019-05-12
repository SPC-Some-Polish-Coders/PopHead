#include "collisionDebugRect.hpp"

#include "Base/gameData.hpp"
#include "Physics/collisionBody.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"

using PopHead::Physics::CollisionDebugRect;

CollisionDebugRect::CollisionDebugRect(PopHead::Base::GameData* gameData, sf::FloatRect rect, PopHead::Physics::CollisionBody* owner)
	:Object(gameData, "collisionDebugRect", Renderer::LayerID::collisionDebug)
	,mShape(sf::Vector2f(rect.width, rect.height))
	,mOwner(owner)
{
	mShape.setPosition(rect.left, rect.top);
}

void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	updateColor();

	if(shouldDisplay())
		target.draw(mShape, states);
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
