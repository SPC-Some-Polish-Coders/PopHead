#include "soundPlayer.hpp"
#include "Utilities/debug.hpp"

using PopHead::Audio::SoundPlayer;

SoundPlayer::SoundPlayer()
	:mVolume(40.f)
{
	loadEverySound();
}

void SoundPlayer::loadEverySound()
{
	mSoundBuffers.load("resources/sounds/barretaShot.wav");
}

void SoundPlayer::playSound(const std::string& filePath)
{
	removeStoppedSounds();

	sf::Sound sound;
	sound.setBuffer(mSoundBuffers.get(filePath));
	sound.setVolume(mVolume);
	mSounds.emplace_back(std::move(sound));
	mSounds.back().play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([](sf::Sound sound) {
		return sound.getStatus() == sf::Sound::Status::Stopped;
	});
}

void SoundPlayer::setVolume(float volume)
{
	mVolume = volume;
	for(auto& sound : mSounds) {
		sound.setVolume(volume);
	}
}

void SoundPlayer::removeEverySound()
{
	mSounds.clear();
}


