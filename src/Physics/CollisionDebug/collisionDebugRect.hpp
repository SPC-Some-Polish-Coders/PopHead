#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_

#include "World/Entity/object.hpp"

namespace PopHead {
namespace Physics {


class CollisionDebugRect : public World::Entity::Object
{
public:
    CollisionDebugRect(Base::GameData*);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setColor(sf::Color color) { mRect.setFillColor(color); }

private:
    sf::RectangleShape mRect;
};


}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_
