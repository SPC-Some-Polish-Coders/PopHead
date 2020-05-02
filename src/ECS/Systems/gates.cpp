#include "pch.hpp"
#include "gates.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

void Gates::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	auto gatesView = mRegistry.view<component::Gate, component::LeverListener>();
	for (auto gate : gatesView)
	{
		auto& [gateDetails, leverListener] = gatesView.get<component::Gate, component::LeverListener>(gate);

		if (leverListener.isActivated)
		{
			leverListener.isActivated = false;
			gateDetails.isOpened = !gateDetails.isOpened;
			if (gateDetails.isOpened)
			{
				mRegistry.remove<component::StaticCollisionBody>(gate);
				mRegistry.remove<component::LightWall>(gate);
				mRegistry.assign_or_replace<component::HiddenForRenderer>(gate);
			}
			else
			{
				mRegistry.assign_or_replace<component::StaticCollisionBody>(gate);
				mRegistry.assign_or_replace<component::LightWall>(gate);
				mRegistry.remove<component::HiddenForRenderer>(gate);
			}
		}
	}
}

}
