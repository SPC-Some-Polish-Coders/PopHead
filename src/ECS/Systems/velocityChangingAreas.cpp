#include "velocityChangingAreas.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void VelocityChangingAreas::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto velocityChaningAreasView = mRegistry.view<component::BodyRect, component::AreaVelocityChangingEffect>();
		auto kinematicObjectsView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::Velocity>();

		for (auto velocityChangingArea : velocityChaningAreasView)
		{
			const auto& [areaBody, velocityChangeEffect] = velocityChaningAreasView.get<component::BodyRect, component::AreaVelocityChangingEffect>(velocityChangingArea);
			
			for (auto kinematicObject : kinematicObjectsView)
			{
				auto& [objectBody, objectVelocity] = kinematicObjectsView.get<component::BodyRect, component::Velocity>(kinematicObject);
				if (areaBody.rect.contains(objectBody.rect.getCenter()))
				{
					objectVelocity.dx *= velocityChangeEffect.areaSpeedMultiplier;
					objectVelocity.dy *= velocityChangeEffect.areaSpeedMultiplier;
				}
			}
		}
	}
}
