#ifndef POPHEAD_AUDIO_MUSICPLAYER_H_
#define POPHEAD_AUDIO_MUSICPLAYER_H_

#include <SFML/Audio.hpp>
#include "MusicData/musicDataHolder.hpp"

namespace ph {

class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();

	void play(const std::string& filePath);
	void stop();

	void setPaused(const bool pause);
	void setMuted(const bool mute);
	bool isMuted() { return mIsMuted; }
	void setVolume(const float volume);
	float getVolume() { return mVolume; }

private:
	MusicDataHolder mMusicDataHolder;
	sf::Music mMusic;
	float mVolume;
	bool mIsMuted;
};

}

#endif // !POPHEAD_AUDIO_MUSICPLAYER_H_
