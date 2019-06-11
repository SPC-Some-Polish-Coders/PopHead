#ifndef POPHEAD_WORLD_OBJECT_H_
#define POPHEAD_WORLD_OBJECT_H_

#include <SFML/Graphics.hpp>
#include <functional>

#include "World/Entity/entity.hpp"
#include "Renderer/layerID.hpp"

namespace ph {

class Object : public Entity, public sf::Drawable
{
public:
	Object(GameData*, std::string name, LayerID);

	void setVisibility(bool visibility, bool recursive = true);
	virtual void setPosition(sf::Vector2f, bool recursive = true);
	virtual void move(sf::Vector2f, bool recursive = true);
	virtual void setScale(sf::Vector2f, bool recursive = true);
	virtual void setRotation(float angle, bool recursive = true);
	virtual void rotate(float angle, bool recursive = true);

	auto getPosition() -> sf::Vector2f const { return mPosition; }
	auto getScale() -> sf::Vector2f const { return mScale; }
	float getRotation() const { return mRotation; }
	auto getLayerID() const -> ph::LayerID { return mLayerID; }
	bool getVisibility() const { return mVisibility; }

private:
	template <typename T>
	void forEachChildWhichIsObject(std::function<void(Object*, T)> func, T param);

protected:
	sf::Vector2f mPosition;
	sf::Vector2f mScale;
	float mRotation;
	bool mVisibility;
	const LayerID mLayerID;
};

}

#include "object.inl"

#endif // !POPHEAD_WORLD_OBJECT_H_
