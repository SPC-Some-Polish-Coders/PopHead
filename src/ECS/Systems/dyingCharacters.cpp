#include "dyingCharacters.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void DyingCharacters::update(float seconds)
	{
		auto view = mRegistry.view<component::Health>();
		for (auto entity : view)
		{
			auto& health = view.get(entity);
			if(health.healthPoints <= 0)
				mRegistry.assign<component::TaggedToDestroy>(entity);
		}
	}
}
