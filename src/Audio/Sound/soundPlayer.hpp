#ifndef POPHEAD_AUDIO_SOUNDPLAYER_H_
#define POPHEAD_AUDIO_SOUNDPLAYER_H_

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
	bool mIsMuted;
};

}

#endif // !POPHEAD_AUDIO_SOUNDPLAYER_H_
