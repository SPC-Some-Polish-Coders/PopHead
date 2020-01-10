#include "lifetime.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void Lifetime::update(float dt)
	{
		PH_PROFILE_FUNCTION(0);

		auto entitiesView = mRegistry.view<component::Lifetime>();
		for (auto entity : entitiesView)
		{
			auto& entityLifetime = entitiesView.get<component::Lifetime>(entity);
			entityLifetime.lifetime -= dt;
			if (entityLifetime.lifetime < 0.f)
				mRegistry.assign<component::TaggedToDestroy>(entity);
		}
	}

}
