#ifndef POPHEAD_AUDIO_MUSICPLAYER_H_
#define POPHEAD_AUDIO_MUSICPLAYER_H_

#include <SFML/Audio.hpp>

namespace PopHead {
namespace Audio {


class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();

	void play(std::string filePath);
	void stop();

	void setPaused(bool pause);
	void setVolume(float volume);
	float getVolume() { return mVolume; }

private:
	sf::Music mMusic;
	float mVolume;
};


}}

#endif // !POPHEAD_AUDIO_MUSICPLAYER_H_
