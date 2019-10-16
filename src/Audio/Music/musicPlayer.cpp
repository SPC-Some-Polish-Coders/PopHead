#include "musicPlayer.hpp"

namespace ph {

MusicPlayer::MusicPlayer()
	:mCurrentThemeFilePath("")
	,mVolume(60.f)
	,mIsMuted(false)
	,mIsPlayingFromMusicState(false)
{
}

MusicPlayer::~MusicPlayer()
{
	mMusic.stop();
}

void MusicPlayer::playFromFile(const std::string& filePath)
{
	if (filePath == mCurrentThemeFilePath)
	{
		adaptVolume();
		return;
	}

	mCurrentThemeFilePath = filePath;

	const MusicData currentThemeData = mMusicDataHolder.getMusicData(filePath);
	const std::string fullFilePath = "resources/" + filePath;

	adaptVolume();
	mMusic.openFromFile(fullFilePath);
	mMusic.setLoop(currentThemeData.mLoop);
	setMuted(mIsMuted);
	mMusic.play();
}

void MusicPlayer::playFromMusicState(const std::string& musicStateName)
{
	auto[filePath, volumeMultiplier] = mMusicStateMachine.getRandomThemeFromState(musicStateName);

	if(filePath == mCurrentThemeFilePath) {
		adaptVolume(volumeMultiplier);
		return;
	}

	mCurrentThemeFilePath = filePath;

	const MusicData currentThemeData = mMusicDataHolder.getMusicData(filePath);
	const std::string fullFilePath = "resources/" + filePath;

	adaptVolume(volumeMultiplier);
	mMusic.openFromFile(fullFilePath);
	mMusic.setLoop(currentThemeData.mLoop);
	setMuted(mIsMuted);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setPaused(const bool pause)
{
	pause ? mMusic.pause() : mMusic.play();
}

void MusicPlayer::setMuted(const bool mute)
{
	mute ? mMusic.setVolume(0.f) : setVolume(mVolume);
	mIsMuted = mute;
}

void MusicPlayer::setVolume(const float volume)
{
	mVolume = volume;
	const float volumeMultiplier = mMusicDataHolder.getCurrentThemeData().mVolumeMultiplier;
	// TODO: Fix this volume multipliers here!
	mMusic.setVolume(volume * volumeMultiplier);
}

void MusicPlayer::adaptVolume(const float volumeMultiplier)
{
	auto volume = mVolume * mMusicDataHolder.getMusicData(mCurrentThemeFilePath).mVolumeMultiplier * volumeMultiplier;
	mMusic.setVolume(volume);
}

}
