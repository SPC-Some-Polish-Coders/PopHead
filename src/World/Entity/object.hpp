#ifndef POPHEAD_WORLD_OBJECT_H_
#define POPHEAD_WORLD_OBJECT_H_

#include <SFML/Graphics.hpp>
#include <functional>

#include "World/Entity/entity.hpp"
#include "Renderer/layerID.hpp"

namespace PopHead {
namespace World {
namespace Entity {


class Object : public Entity, public sf::Drawable
{
public:
    Object(Base::GameData*, std::string name, Renderer::LayerID);

    virtual void onCollision(Object&);

    void setVisibility(bool visibility);
    virtual void setPosition(sf::Vector2f);
	virtual void move(sf::Vector2f);
    virtual void setScale(sf::Vector2f);
    virtual void setRotation(float angle);

    sf::Vector2f getPosition() const;
    sf::Vector2f getScale() const;
    float getRotation() const;
    auto getLayerID() const -> Renderer::LayerID;
    bool getVisibility() const;

private:
	template <typename T>
	void forEachChildWhichIsObject(std::function<void(T)> func, T param);

protected:
    sf::Vector2f mPosition;
    sf::Vector2f mScale;
    float mRotation;
    bool mVisibility;
    const Renderer::LayerID mLayerID;
};


}}}

template <typename T>
void PopHead::World::Entity::Object::forEachChildWhichIsObject(std::function<void(T)> func, T param)
{
	for (auto& child : mChildren) {
		Object* objectPtr = dynamic_cast<Object*>(child.get());
		if (objectPtr == nullptr) {
			;///LOG (WARNING: trying to set visibility to Entity which doesn't inherit from Object, so it doesn't have visibility.)
		}
		else {
			func(param);
		}
	}
}

#endif // !POPHEAD_WORLD_OBJECT_H_
