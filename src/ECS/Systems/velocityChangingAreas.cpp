#include "pch.hpp"
#include "velocityChangingAreas.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using namespace component;

void VelocityChangingAreas::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<BodyRect, AreaVelocityChangingEffect>().each([&]
	(auto areaBody, auto velocityChangingEffect)
	{
		mRegistry.view<KinematicCollisionBody, BodyRect, BodyCircle, Kinematics>().each([&]
		(auto, auto objectBody, auto objectCircle, auto& objectKinematics)
		{
			if(intersect(areaBody, objectBody, objectCircle))
				objectKinematics.vel *= velocityChangingEffect.areaSpeedMultiplier;
		});
	});
}

}
