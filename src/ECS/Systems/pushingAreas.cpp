#include "pushingAreas.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

#include "Utilities/profiling.hpp"

namespace ph::system {

void PushingAreas::update(float dt)
{
	PH_PROFILE_FUNCTION(0);

	if(sPause)
		return;

	auto pushingAreasView = mRegistry.view<component::PushingArea, component::BodyRect>();
	auto kinematicObjects = mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::Velocity>();

	for (auto pushingArea : pushingAreasView)
	{
		const auto& [pushingAreaDetails, areaBody] = pushingAreasView.get<component::PushingArea, component::BodyRect>(pushingArea);

		for (auto kinematicObject : kinematicObjects)
		{
			auto& objectVelocity = kinematicObjects.get<component::Velocity>(kinematicObject);
			const auto& kinematicObjectBody = kinematicObjects.get<component::BodyRect>(kinematicObject);
			if (areaBody.rect.contains(kinematicObjectBody.rect.getCenter()))
			{
				objectVelocity.dx += pushingAreaDetails.pushForce.x;
				objectVelocity.dy += pushingAreaDetails.pushForce.y;
			}
		}
	}
}

}
