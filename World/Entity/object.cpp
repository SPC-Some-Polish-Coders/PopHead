#include "object.hpp"
#include "Base/gameData.hpp"

using PopHead::World::Entity::Object;


Object::Object(PopHead::Base::GameData* gameData, std::string name, PopHead::Renderer::LayerID layerID)
:Entity(PopHead::World::EntityType::object, gameData, name)
,mLayerID(layerID)
{
    mGameData->getRenderer().addObject(this, layerID);
}

void Object::setVisibility(bool visibility, bool recursive)
{
	mVisibility = visibility;

	if (recursive) {
		std::function<void(Object*, bool)> func = [=](Object * object, bool visibility) { object->setVisibility(visibility, recursive); };
		forEachChildWhichIsObject(func, visibility);
	}
}

void Object::setPosition(sf::Vector2f pos, bool recursive)
{
	mPosition = pos;

	if (recursive) {
		std::function<void(Object*, sf::Vector2f)> func = [=](Object * object, sf::Vector2f pos) {object->setPosition(pos, recursive); };
		forEachChildWhichIsObject(func, pos);
	}
}

void Object::move(sf::Vector2f offset, bool recursive)
{
	mPosition += offset;

	if (recursive) {
		std::function<void(Object*, sf::Vector2f)> func = [=](Object * object, sf::Vector2f offset) { object->move(offset, recursive); };
		forEachChildWhichIsObject(func, offset);
	}
}

void Object::setScale(sf::Vector2f scale, bool recursive)
{
	mScale = scale;

	if (recursive) {
		std::function<void(Object*, sf::Vector2f)> func = [=](Object * object, sf::Vector2f scale) { object->setScale(scale, recursive); };
		forEachChildWhichIsObject(func, scale);
	}
}

void Object::setRotation(float angle, bool recursive)
{
	mRotation = angle;

	if (recursive) {
		std::function<void(Object*, float)> func = [=](Object * object, float angle) { object->setRotation(angle, recursive); };
		forEachChildWhichIsObject(func, angle);
	}
}

void Object::rotate(float angle, bool recursive)
{
	mRotation += angle;

	if (recursive) {
		std::function<void(Object*, float)> func = [=](Object * object, float angle) { object->rotate(angle, recursive); };
		forEachChildWhichIsObject(func, angle);
	}
}
