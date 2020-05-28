#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

namespace ph::SoundPlayer
{
	void init();
	void shutdown();

	void playAmbientSound(const std::string& filePath);
	void playSpatialSound(const std::string& filePath, Vec2 soundPosition);

	void setListenerPosition(Vec2 listenerPosition);
	void setMuted(bool muted);
	void setSceneMute(bool mute);
	bool isMuted();
	void setVolume(float volume);
	float getVolume();
	void removeEverySound();
}

