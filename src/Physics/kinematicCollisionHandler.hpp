#ifndef POPHEAD_PHYSICS_KINEMATICCOLLISIONHANDLER_H_
#define POPHEAD_PHYSICS_KINEMATICCOLLISIONHANDLER_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Physics {


class CollisionBody;

class KinematicCollisionHandler
{
public:
	void handleKinematicCollision(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody);

private:
	void init(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody);
	float getForce() const;
	sf::Vector2f getDirectionOfPush() const;
	void applyForce(const sf::Vector2f& forceVector) const;
	void applyForceForBodiesOfEqualsMasses(const sf::Vector2f& forceVector) const;
	void applyForceForBodiesOfEqualsVelocitiesAndMasses(const sf::Vector2f& forceVector) const;

private:
	CollisionBody* mFirstKinematicBody;
	CollisionBody* mSecondKinematicBody;

	float mMass1;
	float mMass2;
};


}}

#endif
