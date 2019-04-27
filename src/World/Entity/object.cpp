#include "object.hpp"
#include "Base/gameData.hpp"

using PopHead::World::Entity::Object;


Object::Object(PopHead::Base::GameData* gameData, std::string name, PopHead::Renderer::LayerID layerID)
:Entity(PopHead::World::EntityType::object, gameData, name)
,mLayerID(layerID)
{
    mGameData->getRenderer().addObject(this, layerID);
}

void Object::onCollision(Object&)
{
}

void Object::setVisibility(bool visibility)
{
	mVisibility = visibility;

	std::function<void(bool)> func = [=](bool visibility) {this->mVisibility = visibility;};
	forEachChildWhichIsObject(func, visibility);
}

void Object::setPosition(sf::Vector2f pos)
{
	mPosition = pos;

	std::function<void(sf::Vector2f)> func = [=](sf::Vector2f pos) {this->mPosition = pos;};
	forEachChildWhichIsObject(func, pos);
}

void Object::move(sf::Vector2f offset)
{
	mPosition.x += offset.x;
	mPosition.y += offset.y;

	std::function<void(sf::Vector2f)> func = [=](sf::Vector2f offset) {
		this->mPosition.x += offset.x;
		this->mPosition.y += offset.y;
	};
	forEachChildWhichIsObject(func, offset);
}

void Object::setScale(sf::Vector2f scale)
{
	mScale = scale;

	std::function<void(sf::Vector2f)> func = [=](sf::Vector2f scale) {this->mScale = scale;};
	forEachChildWhichIsObject(func, scale);
}

void Object::setRotation(float angle)
{
	mRotation = angle;

	std::function<void(float)> func = [=](float angle) {this->mRotation = angle;};
	forEachChildWhichIsObject(func, angle);
}

void Object::rotate(float angle)
{
    mRotation += angle;

    std::function<void(float)> func = [=](float angle) {this->mRotation += angle;};
	forEachChildWhichIsObject(func, angle);
}
