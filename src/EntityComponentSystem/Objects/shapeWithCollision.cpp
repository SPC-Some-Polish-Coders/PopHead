#include "shapeWithCollision.hpp"

#include "Physics/CollisionBody/bodyType.hpp"
#include "gameData.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"

namespace ph {

ShapeWithCollision::ShapeWithCollision(GameData* gameData)
	:Object(gameData, "shapeWithCollision", LayerID::staticEntities)
	,mCollisionBody(mGameData->getPhysicsEngine().createStaticBodyAndGetTheReference(sf::FloatRect(0, 0, 100, 50), "shape with collision"))
	,mShape(sf::Vector2f(100, 50))
{
	mShape.setFillColor(sf::Color::Black);
}

void ShapeWithCollision::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}

void ShapeWithCollision::setPosition(sf::Vector2f position, bool recursive)
{
	mShape.setPosition(position);
	mCollisionBody.setPosition(position);
	Object::setPosition(position, recursive);
}

}