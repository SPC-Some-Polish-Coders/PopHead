#include "musicPlayer.hpp"

using PopHead::Audio::MusicPlayer;

MusicPlayer::MusicPlayer()
	:mVolume(70.f)
	,mMute(true)
{
}

MusicPlayer::~MusicPlayer()
{
	mMusic.stop();
}

void MusicPlayer::play(std::string filePath)
{
	MusicData currentThemeData = musicDataHolder.getMusicData(filePath);
	mMusic.openFromFile(filePath);
	mMusic.setVolume(mVolume * currentThemeData.volumeMultiplier);
	mMusic.setLoop(currentThemeData.loop);
	setMute(mMute);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setPaused(bool pause)
{
	if(pause)
		mMusic.pause();
	else
		mMusic.play();
}

void MusicPlayer::setMute(bool mute)
{
	mMute = mute;
	if(mute)
		mMusic.setVolume(0.f);
	else
		setVolume(mVolume);
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
	float volumeMultiplier = musicDataHolder.getCurrentThemeData().volumeMultiplier;
	mMusic.setVolume(volume * volumeMultiplier);
}
