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

	void setVolumeMultiplierForTheme(const std::string& theme, float multiplier);

	void setPaused(const bool pause);
	void setMuted(const bool mute);
	bool isMuted() { return mIsMuted; }
	void setVolume(const float volume);
	float getVolume() { return mVolume; }

private:
	float getMultiplier(const std::string& theme) const;

private:
	MusicDataHolder mMusicDataHolder;
	std::map<std::string, float> mThemesVolumeMultipliers;
	sf::Music mMusic;
	std::string mCurrentThemeFilePath;
	float mVolume;
	bool mIsMuted;

};

}
