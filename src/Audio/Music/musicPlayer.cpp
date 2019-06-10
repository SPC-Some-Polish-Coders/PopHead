#include "musicPlayer.hpp"

using PopHead::Audio::MusicPlayer;

MusicPlayer::MusicPlayer()
	:mVolume(70.f)
	,mIsMuted(true)
{
}

MusicPlayer::~MusicPlayer()
{
	mMusic.stop();
}

void MusicPlayer::play(const std::string& filePath)
{
	const MusicData currentThemeData = mMusicDataHolder.getMusicData(filePath);
	const std::string fullFilePath = "resources/" + filePath;
	mMusic.openFromFile(fullFilePath);
	mMusic.setVolume(mVolume * currentThemeData.mVolumeMultiplier);
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
	const auto themeData = mMusicDataHolder.getCurrentThemeData();
	const float volumeMultiplier = themeData.mVolumeMultiplier;
	mMusic.setVolume(volume * volumeMultiplier);
}
