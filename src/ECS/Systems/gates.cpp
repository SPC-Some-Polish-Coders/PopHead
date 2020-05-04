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

	mRegistry.view<component::Gate>().each([&]
	(auto entity, component::Gate& gate)
	{
		if(gate.open && !gate.previouslyOpen)
		{
			mRegistry.remove<component::StaticCollisionBody>(entity);
			mRegistry.remove<component::LightWall>(entity);
			mRegistry.assign_or_replace<component::HiddenForRenderer>(entity);
		}
		else if(!gate.open && gate.previouslyOpen)
		{
			mRegistry.assign_or_replace<component::StaticCollisionBody>(entity);
			mRegistry.assign_or_replace<component::LightWall>(entity);
			mRegistry.remove<component::HiddenForRenderer>(entity);
		}
		gate.previouslyOpen = gate.open;
	});
}

}
