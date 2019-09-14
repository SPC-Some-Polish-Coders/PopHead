#pragma once

#include "Resources/resourceHolder.hpp"
#include "Audio/Sound/SoundData/soundDataHolder.hpp"
#include "Audio/Sound/SoundData/soundData.hpp"
#include "Audio/Sound/Spatialization/spatializationManager.hpp"
#include <SFML/Audio.hpp>
#include <list>

namespace ph {

class SoundPlayer
{
public:
	SoundPlayer();

	void playAmbientSound(const std::string& filePath);
	void playSpatialSound(const std::string& filePath, const sf::Vector2f soundPosition);

	void setListenerPosition(const sf::Vector2f listenerPosition){ mSpatializationManager.setListenerPosition(listenerPosition); }
	void setMuted(bool muted);
	bool isMuted() { return mIsMuted; }
	void setVolume(const float volume);
	float getVolume() { return mVolume; }
	float getDefaultVolume() { return mDefaultVolume; }
	void removeEverySound();

private:
	void playSound(const std::string& filePath, const float volume, const bool loop);
	void loadEverySound();
	void removeStoppedSounds();

private:
	std::list<sf::Sound> mSounds;
	SoundBufferHolder mSoundBuffers;
	SoundDataHolder mSoundDataHolder;
	SpatializationManager mSpatializationManager;
	float mVolume;
	float mDefaultVolume;
	bool mIsMuted;
};

}
