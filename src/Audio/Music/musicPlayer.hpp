#pragma once

#include "musicData.hpp"
#include "musicStateMachine.hpp"
#include <SFML/Audio.hpp>
#include <string>

namespace ph {

class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();

	void playFromFile(const std::string& filePath);
	void playFromMusicState(const std::string& musicStateName);
	bool hasMusicState(const std::string& musicStateName) const;
	void stop();

	void setPaused(const bool pause);
	void setMuted(const bool mute);
	bool isMuted() { return mIsMuted; }
	void setVolume(const float volume);
	float getVolume() { return mVolume; }

	auto getMusicStateMachine() -> MusicStateMachine & { return mMusicStateMachine; }

private:
	void adaptVolume(const float volumeMultiplier = 1.f);

private:
	MusicDataHolder mMusicDataHolder;
	MusicStateMachine mMusicStateMachine;
	sf::Music mMusic;
	std::string mCurrentThemeFilePath;
	float mVolume;
	bool mIsMuted;
	bool mIsPlayingFromMusicState;
};

}
