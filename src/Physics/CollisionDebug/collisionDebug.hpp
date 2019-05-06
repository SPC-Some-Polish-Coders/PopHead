#ifndef PH_RELEASE

#ifndef POPHEAD_PHYSICS_COLLISIONSDEBUG_H_
#define POPHEAD_PHYSICS_COLLISIONSDEBUG_H_

#include <vector>
#include "collisionDebugRect.hpp"

namespace PopHead{
namespace Physics{


class CollisionBody;
enum class BodyType;


class CollisionDebug
{
public:
	void turnOn();
	void turnOff();
};


}}

#endif // !POPHEAD_PHYSICS_COLLISIONSDEBUG_H_

#endif // !PH_RELEASE
