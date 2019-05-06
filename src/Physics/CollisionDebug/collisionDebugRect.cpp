#include "collisionDebugRect.hpp"
#include "Base/gameData.hpp"

using PopHead::Physics::CollisionDebugRect;

CollisionDebugRect::CollisionDebugRect(PopHead::Base::GameData* gameData)
	:Object(gameData, "collisionDebugRect", Renderer::LayerID::collisionDebug)
{
}

void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mRect, states);
}
