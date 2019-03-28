#ifndef POPHEAD_WORLD_OBJECT_H_
#define POPHEAD_WORLD_OBJECT_H_

#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "Renderer/layerID.hpp"

namespace PopHead {
namespace World {
namespace Entity {


class Object : public Entity, public sf::Drawable
{
public:
    Object( Renderer::LayerID layerID );

    virtual void draw( sf::RenderTarget&, sf::RenderStates ) const override = 0;

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

    inline auto getLayerID() const -> Renderer::LayerID;

private:
    sf::Vector2f mPosition;
    sf::Vector2f mScale;
    float mRotation;
    ///ColisionObject mColision;
    bool mVisibility;
    const Renderer::LayerID mLayerID;
};


}}}

#endif // !POPHEAD_WORLD_OBJECT_H_
