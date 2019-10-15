
// TEMPORARILY WE DON'T SUPPORT COLLISION DEBUG

//#include "collisionDebugRect.hpp"
//
//#include "gameData.hpp"
//#include "Physics/CollisionBody/collisionBody.hpp"
//#include "Physics/CollisionDebug/collisionDebugSettings.hpp"
//
//namespace ph {
//
//CollisionDebugRect::CollisionDebugRect(sf::FloatRect rect)
//	:mShape({rect.width, rect.height})
//{
//	mShape.setPosition(rect.left, rect.top);
//}
//
//void CollisionDebugRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	target.draw(mShape, states);
//}
//
//void CollisionDebugRect::setColor(sf::Color color)
//{
//	mShape.setFillColor(color);
//}
//
//}