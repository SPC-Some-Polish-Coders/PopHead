#include "dyingCharacters.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void DyingCharacters::update(float seconds)
	{
		auto view = mRegistry.view<component::TaggedToDestroy>();
		mRegistry.destroy(view.begin(), view.end());
	}

}

