#include "entityDestroying.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void EntityDestroying::update(float seconds)
	{
		auto view = mRegistry.view<component::TaggedToDestroy>();
		mRegistry.destroy(view.begin(), view.end());
	}

}

