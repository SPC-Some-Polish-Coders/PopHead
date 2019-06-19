#include "soundPlayer.hpp"

#include <cmath>
#include "Utilities/debug.hpp"

ph::SoundPlayer::SoundPlayer()
	:mVolume(20.f)
	,mIsMuted(true)
{
	loadEverySound();
}

void ph::SoundPlayer::loadEverySound()
{
	mSoundBuffers.load("sounds/barretaShot.wav");
	mSoundBuffers.load("sounds/zombieGetsAttacked.wav");
}

void ph::SoundPlayer::playAmbientSound(const std::string& filePath)
{
	removeStoppedSounds();

	SoundData soundData = mSoundDataHolder.getSoundData(filePath);
	playSound(filePath, mVolume * soundData.mVolumeMultiplier, soundData.mLoop);
}

void ph::SoundPlayer::playSpatialSound(const std::string& filePath, const sf::Vector2f soundPosition)
{
	removeStoppedSounds();

	SoundData soundData = mSoundDataHolder.getSoundData(filePath);
	const float spatialVolume = mSpatializationManager.getSpatialVolume(soundData, soundPosition, mVolume);
	playSound(filePath, spatialVolume, soundData.mLoop);
}

void ph::SoundPlayer::playSound(const std::string& filePath, const float volume, const bool loop)
{
	sf::Sound sound;
	sound.setBuffer(mSoundBuffers.get(filePath));
	sound.setVolume(volume);
	sound.setLoop(loop);
	setMuted(mIsMuted);
	mSounds.emplace_back(std::move(sound));
	mSounds.back().play();
}

void ph::SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([](const sf::Sound sound) {
		return sound.getStatus() == sf::Sound::Status::Stopped;
	});
}

void ph::SoundPlayer::setMuted(bool mute)
{
	static float previousVolume = mVolume;
	setVolume(mute ? 0.f : previousVolume);
	mIsMuted = mute;
}

void ph::SoundPlayer::setVolume(const float volume)
{
	mVolume = volume;
	for(auto& sound : mSounds)
		sound.setVolume(volume);
}

void ph::SoundPlayer::removeEverySound()
{
	mSounds.clear();
}