#pragma once

#include "ECS/system.hpp"

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
		enum class Theme { Exploration, Fight };
		Theme mCurrentlyPlayerTheme = Theme::Exploration;

		MusicPlayer& mMusicPlayer;
		SoundPlayer& mSoundPlayer;
	};
}
