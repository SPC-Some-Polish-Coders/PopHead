#ifndef POPHEAD_WORLD_OBJECT_H_
#define POPHEAD_WORLD_OBJECT_H_

#include <SFML/Graphics.hpp>

#include "World/Entity/entity.hpp"
#include "Renderer/layerID.hpp"

namespace PopHead {
namespace World {
namespace Entity {


class Object : public Entity, public sf::Drawable
{
public:
    Object(EntityType type, Base::GameData* gameData, std::string name, Renderer::LayerID layerID);

    void moveTo(sf::Vector2f);
    virtual void onColision(Object&) = 0;

    void show();
    void hide();

    virtual void setPosition(sf::Vector2f);
    virtual void setScale(sf::Vector2f);
    virtual void setRotation(float angle);

    sf::Vector2f getPosition() const;
    sf::Vector2f getScale() const;
    float getRotation() const;
    ///auto getColision() const -> const ColisionObject&;
    auto getLayerID() const -> Renderer::LayerID;
    bool isVisible() const;

protected:
    sf::Vector2f mPosition;
    sf::Vector2f mScale;
    float mRotation;
    ///ColisionObject mColision;
    bool mVisibility;
    const Renderer::LayerID mLayerID;
};


}}}

#endif // !POPHEAD_WORLD_OBJECT_H_
