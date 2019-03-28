#include "object.hpp"

using PopHead::World::Entity::Object;


Object( Renderer::LayerID layerID )
{

}

void draw( sf::RenderTarget&, sf::RenderStates ) const = 0
{

}

void moveTo(sf::Vector2f)
{

}

virtual void onColision(Object&) = 0
{

}

void show()
{

}

void hide()
{

}

virtual void setPosition(sf::Vector2f)
{

}

virtual void setScale(sf::Vector2f)
{

}

virtual void setRotation(float angle)
{

}

sf::Vector2f getPosition() const
{
    return mPosition;
}

sf::Vector2f getScale() const
{
    return mScale;
}

float getRotation() const
{
    return mRotation;
}

#if 0
auto getColision() const -> const ColisionObject&
{

}
#endif // 0

auto getLayerID() const -> Renderer::LayerID
{
    return mLayerID;
}
