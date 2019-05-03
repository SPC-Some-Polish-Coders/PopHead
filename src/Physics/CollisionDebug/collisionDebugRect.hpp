#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_

#include "World/Entity/object.hpp"

namespace PopHead {
namespace World {
namespace Entity {


class CollisionDebugRect : public Object
{
public:
    CollisionDebugRect();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setColor(sf::Color color) { mRect.setFillColor(color); }

private:
    sf::RectangleShape mRect;
};




}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_
