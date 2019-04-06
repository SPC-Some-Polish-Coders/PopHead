#include "object.hpp"
#include "Base/gameData.hpp"

using PopHead::World::Entity::Object;


Object::Object(Base::GameData* gameData, std::string name, Renderer::LayerID layerID)
:Entity(EntityType::object, gameData, name)
,mLayerID(layerID)
{
    mGameData->getRenderer().addObject(this, layerID);
}

void Object::moveTo(sf::Vector2f)
{
}

void Object::setVisibility(bool visibility)
{
    mVisibility = visibility;
}

void Object::setPosition(sf::Vector2f pos)
{
    mPosition = pos;
}

void Object::setScale(sf::Vector2f scale)
{
    mScale = scale;
}

void Object::setRotation(float angle)
{
    mRotation = angle;
}

sf::Vector2f Object::getPosition() const
{
    return mPosition;
}

sf::Vector2f Object::getScale() const
{
    return mScale;
}

float Object::getRotation() const
{
    return mRotation;
}

#if 0
auto Object::getColision() const -> const ColisionObject&
{
    return mColision;
}
#endif // 0

auto Object::getLayerID() const -> Renderer::LayerID
{
    return mLayerID;
}

bool Object::getVisibility() const
{
    return mVisibility;
}
