#ifndef POPHEAD_PHYSICS_KINEMATICCOLLISIONHANDLER_H_
#define POPHEAD_PHYSICS_KINEMATICCOLLISIONHANDLER_H_

namespace PopHead {
namespace Physics {

class CollisionBody;

class KinematicCollisionHandler
{
public:
	void handleKinematicCollision(CollisionBody* kinematicBodyFirst, CollisionBody* kinematicBodySecond);

private:
	void makeKinematicBodyStickToStaticBody();
	bool isKinematicBodyCollidingOnAxisX();

private:
	CollisionBody* mKinematicBodyFirst;
	CollisionBody* mKinematicBodySecond;
};

}}

#endif
