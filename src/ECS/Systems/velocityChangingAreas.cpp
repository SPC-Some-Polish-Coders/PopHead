#include "velocityChangingAreas.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void VelocityChangingAreas::update(float dt)
	{
		auto velocityChaningAreasView = mRegistry.view<component::Area, component::AreaVelocityChangingEffect>();
		auto kinematicObjectsView = mRegistry.view<component::IsInArea, component::KinematicCollisionBody, component::BodyRect, component::Velocity, component::CharacterSpeed>();

		for (auto velocityChangingArea : velocityChaningAreasView)
		{
			const auto& [areaBody, velocityChangeEffect] = velocityChaningAreasView.get<component::Area, component::AreaVelocityChangingEffect>(velocityChangingArea);
			for (auto kinematicObject : kinematicObjectsView)
			{
				auto& [objectBody, objectVelocity, objectSpeed] = kinematicObjectsView.get<component::BodyRect, component::Velocity, component::CharacterSpeed>(kinematicObject);
				if (areaBody.areaBody.contains(objectBody.rect.getCenter()))
				{
					objectVelocity.dx *= velocityChangeEffect.areaSpeedMultiplier;
					objectVelocity.dy *= velocityChangeEffect.areaSpeedMultiplier;
				}
			}
		}
	}
}
