#ifndef POPHEAD_PHYSICS_COLLISIONSBODY_H_
#define POPHEAD_PHYSICS_COLLISIONSBODY_H_

#include <SFML/Graphics.hpp>

namespace PopHead{
namespace Physics{


class CollisionBody
{
public:
    CollisionBody(sf::FloatRect rect, float mass);

    void move(float speed);

private:
    sf::FloatRect rect;
    float mass;
    sf::Vector2f force;


};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSBODY_H_
