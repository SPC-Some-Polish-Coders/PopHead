#ifndef POPHEAD_PHYSICS_COLLISIONSBODY_H_
#define POPHEAD_PHYSICS_COLLISIONSBODY_H_

#include <SFML/Graphics.hpp>
#include "Base/gameData.hpp"
#include "World/Entity/object.hpp"

namespace PopHead{
namespace Physics{

enum class BodyType;

class CollisionBody
{
public:
    CollisionBody(sf::FloatRect rect, float mass, BodyType, PopHead::World::Entity::Object* const thisPointer, PopHead::Base::GameData*);

    void move(sf::Vector2f velocity);
	void setPosition(sf::Vector2f position);

private:
	void movePhysics();
	void setPositionOfGraphicRepresentation();
	void setPositionToPreviousPosition(CollisionAxis);
	void setPreviousPositionToCurrentPosition();
	sf::FloatRect getPreviousRect();

private:
    sf::FloatRect mRect;
    sf::Vector2f mVelocity;
	sf::Vector2f mPreviousPosition;
    float mMass;
	World::Entity::Object* const pointerToObjectWhichIsOwnerOfThisCollisionBody;

public:
    friend PhysicsEngine;
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSBODY_H_
