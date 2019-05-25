#ifndef POPHEAD_PHYSICS_KINEMATICCOLLISIONHANDLER_H_
#define POPHEAD_PHYSICS_KINEMATICCOLLISIONHANDLER_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Physics {


class CollisionBody;

class KinematicCollisionHandler
{
public:
	void operator()(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody);

private:
	void init(CollisionBody* firstKinematicBody, CollisionBody* secondKinematicBody);
	void calculateForceVector();
	float getForce() const;
	sf::Vector2f getDirectionOfPush() const;
	void applyForceVector() const;
	void applyForceVectorForBodiesOfEqualsMasses() const;
	void applyForceVectorForBodiesOfEqualsVelocitiesAndMasses() const;
	void collisionLog() const;

private:
	CollisionBody* mFirstKinematicBody;
	CollisionBody* mSecondKinematicBody;

	float mMass1;
	float mMass2;
	sf::Vector2f mForceVector;
};


}}

#endif
