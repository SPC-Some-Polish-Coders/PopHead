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
	void calculateMomentums();
	sf::Vector2f getDirectionOfPush() const;
	void applyKinematicCollision();
	void applyShift() const;
	void applyPush() const;

private:
	CollisionBody* mFirstKinematicBody;
	CollisionBody* mSecondKinematicBody;

	float mMass1;
	float mMass2;
	float mMomentum1;
	float mMomentum2;

	float forceMultiplier = 12.5;
};


}}

#endif
