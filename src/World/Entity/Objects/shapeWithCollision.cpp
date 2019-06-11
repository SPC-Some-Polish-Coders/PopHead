#include "shapeWithCollision.hpp"

#include "Physics/CollisionBody/bodyType.hpp"

ph::ShapeWithCollision::ShapeWithCollision(GameData* gameData)
	:Object(gameData, "shapeWithCollision", LayerID::staticEntities)
	,mCollisionBody(sf::FloatRect(0, 0, 100, 50), 200, BodyType::staticBody, this, gameData)
	,mShape(sf::Vector2f(100, 50))
{
	mShape.setFillColor(sf::Color::Black);
}

void ph::ShapeWithCollision::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}

void ph::ShapeWithCollision::setPosition(sf::Vector2f position, bool recursive)
{
	mShape.setPosition(position);
	mCollisionBody.setPosition(position);
	Object::setPosition(position, recursive);
}
