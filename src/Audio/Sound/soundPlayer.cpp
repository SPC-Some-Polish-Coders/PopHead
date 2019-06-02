#include "soundPlayer.hpp"

#include <cmath>
#include "Utilities/debug.hpp"

using PopHead::Audio::SoundPlayer;
using PopHead::Audio::SoundData;

SoundPlayer::SoundPlayer()
	:mVolume(20.f)
{
	loadEverySound();
}

void SoundPlayer::loadEverySound()
{
	mSoundBuffers.load("resources/sounds/barretaShot.wav");
	mSoundBuffers.load("resources/sounds/zombieGetsAttacked.wav");
}

void SoundPlayer::playAmbientSound(const std::string& filePath)
{
	removeStoppedSounds();

	SoundData soundData = mSoundDataHolder.getSoundData(filePath);
	playSound(filePath, mVolume * soundData.mVolumeMultiplier, soundData.mLoop);
}

void SoundPlayer::playSpatialSound(const std::string& filePath, const sf::Vector2f& soundPosition)
{
	removeStoppedSounds();

	SoundData soundData = mSoundDataHolder.getSoundData(filePath);
	float spatialVolume = static_cast<float>(getSpatialVolume(soundData, soundPosition));
	playSound(filePath, spatialVolume, soundData.mLoop);
}

int SoundPlayer::getSpatialVolume(SoundData soundData, const sf::Vector2f& soundPosition)
{
	float distance = hypotf(abs(mListenerPosition.x - soundPosition.x), abs(mListenerPosition.y - soundPosition.y));

	if(distance > soundData.mMax)
		return 0;
	else if(distance < soundData.mMin)
		return static_cast<int>(mVolume * soundData.mVolumeMultiplier);
	else{
		float scope = soundData.mMax - soundData.mMin;
		return static_cast<int>(mVolume * soundData.mVolumeMultiplier * ((scope - (distance - soundData.mMin)) / scope));
	}
}

void SoundPlayer::playSound(const std::string& filePath, float volume, bool loop)
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