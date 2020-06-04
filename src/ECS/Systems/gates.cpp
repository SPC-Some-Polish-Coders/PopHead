#include "pch.hpp"
#include "gates.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using namespace component;

void Gates::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<Gate>().each([&]
	(auto entity, Gate& gate)
	{
		if(gate.open && !gate.previouslyOpen)
		{
			mRegistry.remove<StaticCollisionBody>(entity);
			mRegistry.remove<LightWall>(entity);
			mRegistry.assign_or_replace<HiddenForRenderer>(entity);
		}
		else if(!gate.open && gate.previouslyOpen)
		{
			mRegistry.assign_or_replace<StaticCollisionBody>(entity);
			mRegistry.assign_or_replace<LightWall>(entity);
			mRegistry.remove<HiddenForRenderer>(entity);
		}
		gate.previouslyOpen = gate.open;
	});
}

}
