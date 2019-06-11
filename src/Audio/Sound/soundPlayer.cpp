#include "soundPlayer.hpp"

#include <cmath>
#include "Utilities/debug.hpp"

using ph::Audio::SoundPlayer;
using ph::Audio::SoundData;

SoundPlayer::SoundPlayer()
	:mVolume(20.f)
{
	loadEverySound();
}

void SoundPlayer::loadEverySound()
{
	mSoundBuffers.load("sounds/barretaShot.wav");
	mSoundBuffers.load("sounds/zombieGetsAttacked.wav");
}

void SoundPlayer::playAmbientSound(const std::string& filePath)
{
	removeStoppedSounds();

	SoundData soundData = mSoundDataHolder.getSoundData(filePath);
	playSound(filePath, mVolume * soundData.mVolumeMultiplier, soundData.mLoop);
}

void SoundPlayer::playSpatialSound(const std::string& filePath, const sf::Vector2f soundPosition)
{
	removeStoppedSounds();

	SoundData soundData = mSoundDataHolder.getSoundData(filePath);
	const float spatialVolume = mSpatializationManager.getSpatialVolume(soundData, soundPosition, mVolume);
	playSound(filePath, spatialVolume, soundData.mLoop);
}

void SoundPlayer::playSound(const std::string& filePath, const float volume, const bool loop)
{
	sf::Sound sound;
	sound.setBuffer(mSoundBuffers.get(filePath));
	sound.setVolume(volume);
	sound.setLoop(loop);
	mSounds.emplace_back(std::move(sound));
	mSounds.back().play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([](const sf::Sound sound) {
		return sound.getStatus() == sf::Sound::Status::Stopped;
	});
}

void SoundPlayer::setVolume(const float volume)
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