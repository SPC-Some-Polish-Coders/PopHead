#include "collisionDebugRect.hpp"
#include "Base/gameData.hpp"

using PopHead::Physics::CollisionDebugRect;

CollisionDebugRect::CollisionDebugRect(PopHead::Base::GameData* gameData, sf::FloatRect rect)
	:Object(gameData, "collisionDebugRect", Renderer::LayerID::collisionDebug)
	,mShape(sf::Vector2f(rect.width, rect.height))
{
	mShape.setPosition(rect.left, rect.top);
}
	
void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape, states);
}
