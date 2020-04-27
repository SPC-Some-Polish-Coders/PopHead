#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

namespace ph::SoundPlayer
{
	void init();
	void shutdown();

	void playAmbientSound(const std::string& filePath);
	void playSpatialSound(const std::string& filePath, sf::Vector2f soundPosition);

	void setListenerPosition(sf::Vector2f listenerPosition);
	void setMuted(bool muted);
	void setSceneMute(bool mute);
	bool isMuted();
	void setVolume(float volume);
	float getVolume();
	void removeEverySound();
}

