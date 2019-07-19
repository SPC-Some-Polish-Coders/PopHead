#pragma once

#include "entity.hpp"
#include "Renderer/layerID.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

namespace ph {

class GameData;

class Object : public Entity, public sf::Drawable
{
public:
	Object(GameData*, std::string name, LayerID);
	virtual ~Object();

	void setVisibility(bool visibility, bool recursive = true);
	virtual void setPosition(sf::Vector2f, bool recursive = true);
	virtual void move(sf::Vector2f, bool recursive = true);
	virtual void setScale(sf::Vector2f, bool recursive = true);
	virtual void setRotation(float angle, bool recursive = true);
	virtual void rotate(float angle, bool recursive = true);

	auto getPosition() const -> sf::Vector2f { return mPosition; }
	auto getScale() const -> sf::Vector2f { return mScale; }
	float getRotation() const { return mRotation; }
	auto getLayerID() const -> ph::LayerID { return mLayerID; }
	bool getVisibility() const { return mVisibility; }

private:
	template <typename T>
	void forEachChildWhichIsObject(std::function<void(Object*, T)> func, T param);

protected:
	sf::Vector2f mPosition;
	sf::Vector2f mScale;
	GameData* mGameData;
	const LayerID mLayerID;
	float mRotation;
	bool mVisibility;
};

}

#include "object.inl"
