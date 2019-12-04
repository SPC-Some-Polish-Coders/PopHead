#pragma once

#include "ECS/system.hpp"
#include <vector>

namespace ph {
	class MusicPlayer;
	class SoundPlayer;
}

namespace ph::system {

	class AudioSystem : public System
	{
	public:
		AudioSystem(entt::registry& registry, MusicPlayer&, SoundPlayer&);

		void update(float dt) override;

	private:
		MusicPlayer& mMusicPlayer;
		SoundPlayer& mSoundPlayer;

		enum class Theme { Exploration, Fight };
		Theme mCurrentlyPlayerTheme = Theme::Exploration;

		std::vector<float> mSoundDistancesFromPlayer;
	};
}
