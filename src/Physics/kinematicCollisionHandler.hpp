#ifndef POPHEAD_PHYSICS_KINEMATICCOLLISIONHANDLER_H_
#define POPHEAD_PHYSICS_KINEMATICCOLLISIONHANDLER_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Physics {


class CollisionBody;

class KinematicCollisionHandler
{
public:
	void handleKinematicCollision(CollisionBody* kinematicBodyFirst, CollisionBody* kinematicBodySecond);

private:
	sf::Vector2f getDirectionOfPush() const;
	float getForce() const;
	void applyForce(sf::Vector2f forceVector) const;

private:
	CollisionBody* mFirstKinematicBody;
	CollisionBody* mSecondKinematicBody;
};


}}

#endif
