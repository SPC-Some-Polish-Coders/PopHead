#include "pch.hpp"
#include "velocityChangingAreas.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

namespace ph::system {

using namespace component;

void VelocityChangingAreas::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<AreaVelocityChangingEffect, InsideSimRegion, BodyRect>().each([&]
	(auto velocityChangingEffect, auto, auto areaBody)
	{
		mRegistry.view<KinematicCollisionBody, InsideSimRegion, BodyRect, BodyCircle, Kinematics>().each([&]
		(auto, auto, auto objectBody, auto objectCircle, auto& objectKinematics)
		{
			if(intersect(areaBody, objectBody, objectCircle))
				objectKinematics.vel *= velocityChangingEffect.areaSpeedMultiplier;
		});
	});
}

}
