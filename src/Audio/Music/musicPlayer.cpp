#include "musicPlayer.hpp"
#include "musicData.hpp"
#include "musicStateMachine.hpp"
#include <SFML/Audio.hpp>
#include <memory>

namespace ph {

namespace MusicPlayer
{

namespace {
	MusicDataHolder musicDataHolder;
	MusicStateMachine musicStateMachine;
	std::unique_ptr<sf::Music> music;
	std::string currentThemeFilePath;
	float musicVolume = 50.f;
	bool isMusicMuted = false;
	bool isPlayingFromMusicState = false;
}

static void adaptVolume(float volumeMultiplier = 1.f)
{
	auto volume = musicVolume * musicDataHolder.getMusicData(currentThemeFilePath).mVolumeMultiplier * volumeMultiplier;
	music->setVolume(volume);
}

void init()
{
	music = std::make_unique<sf::Music>();
}

void playFromFile(const std::string& filePath)
{
	if (filePath == currentThemeFilePath)
	{
		adaptVolume();
		return;
	}

	currentThemeFilePath = filePath;

	const MusicData currentThemeData = musicDataHolder.getMusicData(filePath);
	const std::string fullFilePath = "resources/" + filePath;

	adaptVolume();
	music->openFromFile(fullFilePath);
	music->setLoop(currentThemeData.mLoop);
	setMuted(isMuted);
	music->play();
}

void playFromMusicState(const std::string& musicStateName)
{
	auto[filePath, volumeMultiplier] = musicStateMachine.getRandomThemeFromState(musicStateName);

	if(filePath == currentThemeFilePath) {
		adaptVolume(volumeMultiplier);
		return;
	}

	currentThemeFilePath = filePath;

	const MusicData currentThemeData = musicDataHolder.getMusicData(filePath);
	const std::string fullFilePath = "resources/" + filePath;

	adaptVolume(volumeMultiplier);
	music->openFromFile(fullFilePath);
	music->setLoop(currentThemeData.mLoop);
	setMuted(isMuted);
	music->play();
}

bool hasMusicState(const std::string& musicStateName)
{
	return musicStateMachine.hasMusicState(musicStateName);
}

void stop()
{
	music->stop();
}

void setPaused(bool pause)
{
	pause ? music->pause() : music->play();
}

void setMuted(bool mute)
{
	mute ? music->setVolume(0.f) : setVolume(musicVolume);
	isMusicMuted = mute;
}

bool isMuted()
{
	return isMusicMuted;
}

void setVolume(float volume)
{
	musicVolume = volume;
	const float volumeMultiplier = musicDataHolder.getCurrentThemeData().mVolumeMultiplier;
	music->setVolume(volume * volumeMultiplier);
}

float getVolume()
{
	return musicVolume;
}

auto getMusicStateMachine() -> MusicStateMachine &
{
	return musicStateMachine;
}

}}

