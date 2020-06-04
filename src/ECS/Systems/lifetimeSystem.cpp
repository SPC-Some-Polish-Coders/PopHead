#include "pch.hpp"
#include "lifetimeSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

using namespace component;

void LifetimeSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<Lifetime>().each([&]
	(auto entity, auto& lifetime)
	{
		lifetime.lifetime -= dt;
		if(lifetime.lifetime < 0.f)
			mRegistry.assign<TaggedToDestroy>(entity);
	});
}

}
