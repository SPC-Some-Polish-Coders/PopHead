#pragma once

#include "MusicData/musicDataHolder.hpp"
#include <SFML/Audio.hpp>
#include <string>

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
	float getDefaultVolume() { return mDefaultVolume; }

private:
	MusicDataHolder mMusicDataHolder;
	sf::Music mMusic;
	std::string mCurrentThemeFilePath;
	float mVolume;
	float mDefaultVolume;
	bool mIsMuted;

};

}
