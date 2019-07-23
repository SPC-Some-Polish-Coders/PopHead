#include "drawableGameObject.hpp"
#include "gameData.hpp"

namespace ph {

DrawableGameObject::DrawableGameObject(GameData* gameData, std::string name, LayerID layerID)
	:GameObject(name)
	,mPosition(0, 0)
	,mScale(0, 0)
	,mGameData(gameData)
	,mLayerID(layerID)
	,mRotation(0)
	,mVisibility(true)
{
    mGameData->getRenderer().addObject(this, layerID);
}

void DrawableGameObject::setVisibility(bool visibility, bool recursive)
{
	mVisibility = visibility;

	if (recursive) {
		std::function<void(DrawableGameObject*, bool)> func = [=](DrawableGameObject * object, bool visibility) {
			object->setVisibility(visibility, recursive); 
		};
		forEachChildWhichIsDrawableGameObject(func, visibility);
	}
}

void DrawableGameObject::setPosition(sf::Vector2f pos, bool recursive)
{
	mPosition = pos;

	if (recursive) {
		std::function<void(DrawableGameObject*, sf::Vector2f)> func = [=](DrawableGameObject * object, sf::Vector2f pos) {
			object->setPosition(pos, recursive); 
		};
		forEachChildWhichIsDrawableGameObject(func, pos);
	}
}

void DrawableGameObject::move(sf::Vector2f offset, bool recursive)
{
	mPosition += offset;

	if (recursive) {
		std::function<void(DrawableGameObject*, sf::Vector2f)> func = [=](DrawableGameObject * object, sf::Vector2f offset) {
			object->move(offset, recursive); 
		};
		forEachChildWhichIsDrawableGameObject(func, offset);
	}
}

void DrawableGameObject::setScale(sf::Vector2f scale, bool recursive)
{
	mScale = scale;

	if (recursive) {
		std::function<void(DrawableGameObject*, sf::Vector2f)> func = [=](DrawableGameObject * object, sf::Vector2f scale) {
			object->setScale(scale, recursive); 
		};
		forEachChildWhichIsDrawableGameObject(func, scale);
	}
}

void DrawableGameObject::setRotation(float angle, bool recursive)
{
	mRotation = angle;

	if (recursive) {
		std::function<void(DrawableGameObject*, float)> func = [=](DrawableGameObject * object, float angle) { 
			object->setRotation(angle, recursive); 
		};
		forEachChildWhichIsDrawableGameObject(func, angle);
	}
}

void DrawableGameObject::rotate(float angle, bool recursive)
{
	mRotation += angle;

	if (recursive) {
		std::function<void(DrawableGameObject*, float)> func = [=](DrawableGameObject * object, float angle) { 
			object->rotate(angle, recursive); 
		};
		forEachChildWhichIsDrawableGameObject(func, angle);
	}
}

}