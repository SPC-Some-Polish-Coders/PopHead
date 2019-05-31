#ifndef POPHEAD_AUDIO_SOUNDPLAYER_H_
#define POPHEAD_AUDIO_SOUNDPLAYER_H_

#include "Resources/resourceHolder.hpp"
#include <SFML/Audio.hpp>
#include <list>

namespace PopHead {
namespace Audio {

class SoundPlayer
{
public:
	SoundPlayer();

	void playSound(const std::string& filePath);
	void setVolume(float volume);
	void removeEverySound();

private:
	void loadEverySound();
	void removeStoppedSounds();

private:
	Resources::SoundBufferHolder mSoundBuffers;
	std::list<sf::Sound> mSounds;
	float mVolume;
};

}}

#endif // !POPHEAD_AUDIO_SOUNDPLAYER_H_
