#pragma once

#include "ECS/system.hpp"
#include <vector>

namespace ph::system {

	class AudioSystem : public System
	{
	public:
		AudioSystem(entt::registry& registry);

		void update(float dt) override;

	private:
		enum class Theme { Exploration, Fight };
		Theme mCurrentlyPlayerTheme = Theme::Exploration;

		std::vector<float> mSoundDistancesFromPlayer;
	};
}
