#include "pch.hpp"
#include "entityDestroying.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void EntityDestroying::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto view = mRegistry.view<component::TaggedToDestroy>();
		mRegistry.destroy(view.begin(), view.end());
	}

}
