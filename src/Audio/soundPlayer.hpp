#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

namespace ph::SoundPlayer
{
	void init();
	void shutdown();

	void playAmbientSound(const std::string& filePath);
	void playSpatialSound(const std::string& filePath, Vec2 soundPosition);

	void setListenerPosition(Vec2);
	void setVolume(float volume);
	float getVolume();
}

