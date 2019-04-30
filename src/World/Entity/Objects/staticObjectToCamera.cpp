#include "staticObjectToCamera.hpp"

#include <SFML/Graphics.hpp>

using PopHead::World::Entity::StaticObjectToCamera;

StaticObjectToCamera::StaticObjectToCamera(PopHead::Base::GameData* gameData)
:Object(gameData, "staticObjectToCamera", Renderer::LayerID::GUI)
,mShape(sf::Vector2f(300, 100))
{
	mShape.setFillColor(sf::Color::Red);
	mShape.setPosition(-500, -500);
}

void StaticObjectToCamera::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}

void StaticObjectToCamera::move(sf::Vector2f offset)
{
	mShape.move(offset);
	Object::move(offset);
}

