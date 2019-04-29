#ifndef POPHEAD_PHYSICS_COLLISIONSBODY_H_
#define POPHEAD_PHYSICS_COLLISIONSBODY_H_

#include <SFML/Graphics.hpp>

namespace PopHead{
namespace Physics{


class CollisionBody
{
public:
    CollisionBody(sf::FloatRect rect, float mass);

    void addToForce(sf::Vector2f force);

private:
    sf::FloatRect mRect;
    float mMass;
    sf::Vector2f mForce;
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSBODY_H_
