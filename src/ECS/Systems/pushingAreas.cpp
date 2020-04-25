#include "pushingAreas.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

#include "Utilities/profiling.hpp"

namespace ph::system {

void PushingAreas::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	auto pushingAreasView = mRegistry.view<component::PushingArea, component::BodyRect>();
	auto kinematicObjects = mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::Kinematics>();

	for (auto pushingArea : pushingAreasView)
	{
		const auto& [pushingAreaDetails, areaBody] = pushingAreasView.get<component::PushingArea, component::BodyRect>(pushingArea);

		for (auto kinematicObject : kinematicObjects)
		{
			auto& objectKinematics = kinematicObjects.get<component::Kinematics>(kinematicObject);
			const auto& kinematicObjectBody = kinematicObjects.get<component::BodyRect>(kinematicObject);
			if (areaBody.rect.contains(kinematicObjectBody.rect.getCenter()))
			{
				objectKinematics.vel += pushingAreaDetails.pushForce;
			}
		}
	}
}

}
