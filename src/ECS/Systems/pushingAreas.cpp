#include "pushingAreas.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"

namespace ph::system {

void PushingAreas::update(float seconds)
{
	auto pushingAreasView = mRegistry.view<component::PushingArea, component::Area>();
	auto kinematicObjects = mRegistry.view<component::IsInArea, component::KinematicCollisionBody, component::BodyRect, component::Velocity, component::CharacterSpeed>();

	for (auto pushingArea : pushingAreasView)
	{
		const auto& [pushingAreaDetails, areaBody] = pushingAreasView.get<component::PushingArea, component::Area>(pushingArea);
		for (auto kinematicObject : kinematicObjects)
		{
			auto& [kinematicObjectBody, objectVelocity, characterSpeed] = kinematicObjects.get<component::BodyRect, component::Velocity, component::CharacterSpeed>(kinematicObject);
			if (areaBody.areaBody.contains(kinematicObjectBody.rect.getCenter()))
			{
				const auto velocityFromPushingArea = pushingAreaDetails.pushDirection * pushingAreaDetails.velocityMultiplier * characterSpeed.speed;
				objectVelocity.dx += velocityFromPushingArea.x;
				objectVelocity.dy += velocityFromPushingArea.y;
			}
		}
	}
}

}
