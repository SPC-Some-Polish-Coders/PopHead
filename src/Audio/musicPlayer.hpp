#pragma once

#include <string>

namespace ph {

struct MusicState
{
	std::vector<std::string> filepaths;
	std::vector<float> volumeMultipliers;
};

namespace MusicPlayer
{
	void init();
	void shutdown();

	void playFromFile(const std::string& filePath);
	void playFromMusicState(const std::string& musicStateName);
	void stop();

	void addMusicState(const std::string& stateName, MusicState& state);
	bool hasMusicState(const std::string& musicStateName);
	void clearMusicStates();

	void setPaused(bool pause);
	void setVolume(float volume);
	float getVolume();
}

}
