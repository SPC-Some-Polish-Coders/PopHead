#include "collisionDebugRect.hpp"

#include "Base/gameData.hpp"
#include "Physics/collisionBody.hpp"
#include "Physics/CollisionDebug/collisionDebugSettings.hpp"

using PopHead::Physics::CollisionDebugRect;

bool CollisionDebugRect::shouldUpdateTheSettings = true;

CollisionDebugRect::CollisionDebugRect(PopHead::Base::GameData* gameData, sf::FloatRect rect, PopHead::Physics::CollisionBody* owner)
	:Object(gameData, "collisionDebugRect", Renderer::LayerID::collisionDebug)
	,mShape(sf::Vector2f(rect.width, rect.height))
	,mOwner(owner)
{
	mShape.setPosition(rect.left, rect.top);
}

void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (shouldUpdateTheSettings)
		updateSettings();

	using PopHead::Physics::CollisionDebugSettings;

	if(CollisionDebugSettings::getInstance().shouldDisplay(mOwner->getBodyType()))
		target.draw(mShape, states);
}

void CollisionDebugRect::updateSettings() const
{
	auto& settings = CollisionDebugSettings::getInstance();
	sf::Color newColor = settings.getFillColor(mOwner->getBodyType());
	mShape.setFillColor(newColor);
}
