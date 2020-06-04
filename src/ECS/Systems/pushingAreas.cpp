#include "pch.hpp"
#include "pushingAreas.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using namespace component;

void PushingAreas::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<PushingArea, BodyRect>().each([&]
	(auto pushingArea, auto pushingAreaBody)
	{
		mRegistry.view<KinematicCollisionBody, BodyRect, BodyCircle, Kinematics>().each([&]
		(auto, auto objectBody, auto objectCircle, auto& objectKinematics)
		{
			if(intersect(pushingAreaBody, objectBody, objectCircle))
				objectKinematics.vel += pushingArea.pushForce;
		});
	});
}

}
