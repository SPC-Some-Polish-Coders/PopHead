#include "musicPlayer.hpp"

using PopHead::Audio::MusicPlayer;

MusicPlayer::MusicPlayer()
	:mVolume(10.f)
{
}

MusicPlayer::~MusicPlayer()
{
	mMusic.stop();
}

void MusicPlayer::play(std::string filePath)
{
	mMusic.openFromFile(filePath);
	mMusic.setVolume(mVolume);
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
	mMusic.setVolume(volume);
}
