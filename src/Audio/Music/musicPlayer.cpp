#include "musicPlayer.hpp"

using PopHead::Audio::MusicPlayer;

MusicPlayer::MusicPlayer()
	:mVolume(70.f)
{
}

MusicPlayer::~MusicPlayer()
{
	mMusic.stop();
}

void MusicPlayer::play(std::string filePath)
{
	currentThemeData = getMusicData(filePath);
	mMusic.openFromFile(filePath);
	mMusic.setVolume(mVolume * currentThemeData.volumeMultiplier);
	mMusic.setLoop(currentThemeData.loop);
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

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
	mMusic.setVolume(volume * currentThemeData.volumeMultiplier);
}
