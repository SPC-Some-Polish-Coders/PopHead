#include "velocityChangingAreas.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void VelocityChangingAreas::update(float dt)
	{
		auto velocityChaningAreasView = mRegistry.view<component::Area, component::AreaVelocityChangingEffect>();
		auto kinematicObjectsView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::Velocity>();

		for (auto velocityChangingArea : velocityChaningAreasView)
		{
			const auto& [areaBody, velocityChangeEffect] = velocityChaningAreasView.get<component::Area, component::AreaVelocityChangingEffect>(velocityChangingArea);
			
			for (auto kinematicObject : kinematicObjectsView)
			{
				auto& [objectBody, objectVelocity] = kinematicObjectsView.get<component::BodyRect, component::Velocity>(kinematicObject);
				if (areaBody.areaBody.contains(objectBody.rect.getCenter()))
				{
					objectVelocity.dx *= velocityChangeEffect.areaSpeedMultiplier;
					objectVelocity.dy *= velocityChangeEffect.areaSpeedMultiplier;
				}
			}
		}
	}
}
