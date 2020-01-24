#pragma once

#include "ECS/system.hpp"
#include <vector>

namespace ph {
	class MusicPlayer;
}

namespace ph::system {

	class AudioSystem : public System
	{
	public:
		AudioSystem(entt::registry& registry, MusicPlayer&);

		void update(float dt) override;

	private:
		MusicPlayer& mMusicPlayer;

		enum class Theme { Exploration, Fight };
		Theme mCurrentlyPlayerTheme = Theme::Exploration;

		std::vector<float> mSoundDistancesFromPlayer;
	};
}
