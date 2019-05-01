#include "object.hpp"
#include "Base/gameData.hpp"

using PopHead::World::Entity::Object;


Object::Object(PopHead::Base::GameData* gameData, std::string name, PopHead::Renderer::LayerID layerID)
:Entity(PopHead::World::EntityType::object, gameData, name)
,mLayerID(layerID)
{
    mGameData->getRenderer().addObject(this, layerID);
}

void Object::setVisibility(bool visibility)
{
	mVisibility = visibility;

	std::function<void(Object*, bool)> func = [=](Object * object, bool visibility) {object->setVisibility(visibility);};
	forEachChildWhichIsObject(func, visibility);
}

void Object::setPosition(sf::Vector2f pos, bool recursive)
{
	mPosition = pos;

	if (recursive) {
		std::function<void(Object*, sf::Vector2f)> func = [=](Object * object, sf::Vector2f pos) {object->setPosition(pos, recursive); };
		forEachChildWhichIsObject(func, pos);
	}
}

void Object::move(sf::Vector2f offset)
{
	mPosition += offset;
	std::function<void(Object*, sf::Vector2f)> func = [](Object* object, sf::Vector2f offset) {
		object->move(offset);
	};
	forEachChildWhichIsObject(func, offset);
}

void Object::setScale(sf::Vector2f scale)
{
	mScale = scale;

	std::function<void(Object*, sf::Vector2f)> func = [=](Object * object, sf::Vector2f scale) {object->setScale(scale); };
	forEachChildWhichIsObject(func, scale);
}

void Object::setRotation(float angle)
{
	mRotation = angle;

	std::function<void(Object*, float)> func = [=](Object * object, float angle) {object->setRotation(angle); };
	forEachChildWhichIsObject(func, angle);
}

void Object::rotate(float angle)
{
	mRotation += angle;

	std::function<void(Object*, float)> func = [=](Object* object, float angle) {object->rotate(angle);};
	forEachChildWhichIsObject(func, angle);
}
