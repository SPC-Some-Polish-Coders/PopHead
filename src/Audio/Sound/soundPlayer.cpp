#include "soundPlayer.hpp"

using PopHead::Audio::SoundPlayer;

SoundPlayer::SoundPlayer()
	:mVolume(10.f)
{
	loadEverySound();
}

void SoundPlayer::loadEverySound()
{
	mSoundBuffers.load("resources/sounds/barretaShot.wav");
}

void SoundPlayer::playSound(const std::string& filePath)
{
	sf::Sound sound;
	sound.setBuffer(mSoundBuffers.get(filePath));
	sound.setVolume(mVolume);
	mSounds.emplace_back(sound);
}

void SoundPlayer::removeStoppedSounds()
{
}

void SoundPlayer::setVolume(float volume)
{
	mVolume = volume;
	for(auto& sound : mSounds) {
		sound.setVolume(volume);
	}
}


