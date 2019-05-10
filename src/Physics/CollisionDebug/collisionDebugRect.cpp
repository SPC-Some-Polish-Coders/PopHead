#include "collisionDebugRect.hpp"

#include "Base/gameData.hpp"
#include "Physics/collisionBody.hpp"

using PopHead::Physics::CollisionDebugRect;

bool CollisionDebugRect::mShouldDisplay = false;
bool CollisionDebugRect::mShouldDisplayKinematicBodies = false;
bool CollisionDebugRect::mShouldDisplayStaticBodies = false;

CollisionDebugRect::CollisionDebugRect(PopHead::Base::GameData* gameData, sf::FloatRect rect, PopHead::Physics::CollisionBody* owner)
	:Object(gameData, "collisionDebugRect", Renderer::LayerID::collisionDebug)
	,mShape(sf::Vector2f(rect.width, rect.height))
	,mOwner(owner)
{
	mShape.setPosition(rect.left, rect.top);
}
	
void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(shouldDisplay())
		target.draw(mShape, states);
}

bool CollisionDebugRect::shouldDisplay() const
{
	if (mOwner->getBodyType() == BodyType::kinematicBody && !mShouldDisplayKinematicBodies)
		return false;
	if (mOwner->getBodyType() == BodyType::staticBody && !mShouldDisplayStaticBodies)
		return false;

	return mShouldDisplay;
}
