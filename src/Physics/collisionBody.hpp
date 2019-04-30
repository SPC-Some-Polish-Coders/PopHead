#ifndef POPHEAD_PHYSICS_COLLISIONSBODY_H_
#define POPHEAD_PHYSICS_COLLISIONSBODY_H_

#include <SFML/Graphics.hpp>
#include "Base/gameData.hpp"

namespace PopHead{
namespace Physics{

enum class BodyType;

class CollisionBody
{
public:
    CollisionBody(sf::FloatRect rect, float mass, BodyType, PopHead::Base::GameData*);

    void move(sf::Vector2f velocity);

private:
    sf::FloatRect mRect;
    sf::Vector2f mVelocity;
    float mMass;

public:
    friend PhysicsEngine;
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSBODY_H_
