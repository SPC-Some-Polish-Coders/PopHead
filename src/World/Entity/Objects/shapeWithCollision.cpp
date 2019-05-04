#include "shapeWithCollision.hpp"

#include "Physics/bodyType.hpp"

using PopHead::World::Entity::ShapeWithCollision;

ShapeWithCollision::ShapeWithCollision(PopHead::Base::GameData* gameData)
	:Object(gameData, "shapeWithCollision", Renderer::LayerID::staticEntities)
	,mCollisionBody(sf::FloatRect(0, 0, 100, 50), 200, Physics::BodyType::staticBody, this, gameData)
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