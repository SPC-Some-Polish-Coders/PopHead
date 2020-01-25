#pragma once

#include <string>

namespace ph {

class MusicStateMachine;

namespace MusicPlayer
{
	void init();

	void playFromFile(const std::string& filePath);
	void playFromMusicState(const std::string& musicStateName);
	bool hasMusicState(const std::string& musicStateName);
	void stop();

	void setPaused(bool pause);
	void setMuted(bool mute);
	bool isMuted();
	void setVolume(float volume);
	float getVolume();
	auto getMusicStateMachine() -> MusicStateMachine&;
}

}
