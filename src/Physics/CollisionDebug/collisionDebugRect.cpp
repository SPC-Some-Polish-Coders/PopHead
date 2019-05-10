#include "collisionDebugRect.hpp"
#include "Base/gameData.hpp"

using PopHead::Physics::CollisionDebugRect;

bool CollisionDebugRect::mShouldDisplay = false;

CollisionDebugRect::CollisionDebugRect(PopHead::Base::GameData* gameData, sf::FloatRect rect, PopHead::Physics::CollisionBody* owner)
	:Object(gameData, "collisionDebugRect", Renderer::LayerID::collisionDebug)
	,mShape(sf::Vector2f(rect.width, rect.height))
	,owner(owner)
{
	mShape.setPosition(rect.left, rect.top);
}
	
void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mShouldDisplay)
		target.draw(mShape, states);
}
