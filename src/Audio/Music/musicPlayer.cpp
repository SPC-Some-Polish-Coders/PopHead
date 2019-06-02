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
	MusicData currentThemeData = musicDataHolder.getMusicData(filePath);
	mMusic.openFromFile(filePath);
	mMusic.setVolume(mVolume * currentThemeData.mVolumeMultiplier);
	mMusic.setLoop(currentThemeData.mLoop);
	setMute(mIsMuted);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setPaused(bool pause)
{
	pause ? mMusic.pause() : mMusic.play();
}

void MusicPlayer::setMute(bool mute)
{
	mute ? mMusic.setVolume(0.f) : setVolume(mVolume);
	mIsMuted = mute;
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
	auto themeData = musicDataHolder.getCurrentThemeData();
	float volumeMultiplier = themeData.mVolumeMultiplier;
	mMusic.setVolume(volume * volumeMultiplier);
}
