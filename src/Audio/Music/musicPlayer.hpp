#ifndef POPHEAD_AUDIO_MUSICPLAYER_H_
#define POPHEAD_AUDIO_MUSICPLAYER_H_

#include <SFML/Audio.hpp>
#include "MusicData/musicDataHolder.hpp"

namespace PopHead {
namespace Audio {


class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();

	void play(const std::string& filePath);
	void stop();

	void setPaused(bool pause);
	void setMute(bool mute);
	bool isMuted() { return mIsMuted; }
	void setVolume(float volume);
	float getVolume() { return mVolume; }

private:
	MusicDataHolder musicDataHolder;
	sf::Music mMusic;
	float mVolume;
	bool mIsMuted;
};


}}

#endif // !POPHEAD_AUDIO_MUSICPLAYER_H_
