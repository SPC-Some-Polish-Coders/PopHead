#ifndef POPHEAD_PHYSICS_STATICCOLLISIONHANDLER_H_
#define POPHEAD_PHYSICS_STATICCOLLISIONHANDLER_H_

namespace PopHead {
namespace Physics {

class CollisionBody;

class StaticCollisionHandler
{
public:
	void handleStaticCollision(CollisionBody* kinematicBody, CollisionBody* staticBody);

private:
	void makeKinematicBodyStickToStaticBody();
	bool isKinematicBodyCollidingOnAxisX();

private:
	CollisionBody* mKinematicBody;
	CollisionBody* mStaticBody;

};


}}

#endif // !POPHEAD_PHYSICS_STATICCOLLISIONHANDLER_H_

