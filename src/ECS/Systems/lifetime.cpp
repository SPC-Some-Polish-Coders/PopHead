#include "lifetime.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void Lifetime::update(float seconds)
	{
		auto entitiesView = mRegistry.view<component::Lifetime>();
		for (auto entity : entitiesView)
		{
			auto& entityLifetime = entitiesView.get<component::Lifetime>(entity);
			entityLifetime.lifetime -= seconds;
			if (entityLifetime.lifetime < 0.f)
				mRegistry.assign<component::TaggedToDestroy>(entity);
		}
	}

}
