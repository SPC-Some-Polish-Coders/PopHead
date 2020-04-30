#include "pch.hpp"
#include "soundPlayer.hpp"
#include "Resources/soundBufferHolder.hpp"
#include "Audio/Sound/soundData.hpp"
#include "Audio/Sound/spatializationManager.hpp"

namespace ph::SoundPlayer
{

namespace {
	std::list<sf::Sound> sounds;
	SoundDataHolder soundDataHolder;
	SpatializationManager spatializationManager;
	float soundVolume = 15.f;
	bool areSoundsMuted = false;
	bool sceneMute = false;
}

static void removeStoppedSounds()
{
	sounds.remove_if([](const sf::Sound& sound) {
		return sound.getStatus() == sf::Sound::Status::Stopped;
	});
}

static void playSound(const std::string& filePath, float volume, bool loop)
{
	sf::Sound sound;
	sound.setBuffer(getSoundBuffer(filePath));
	sound.setVolume(volume);
	sound.setLoop(loop);
	sounds.emplace_back(std::move(sound));
	sounds.back().play();
}

void init()
{
	setMuted(false);

	loadSoundBuffer("sounds/swordAttack.wav");
	loadSoundBuffer("sounds/zombieGrowl1.ogg");
	loadSoundBuffer("sounds/zombieGrowl2.ogg");
	loadSoundBuffer("sounds/zombieGrowl3.ogg");
	loadSoundBuffer("sounds/zombieGrowl4.ogg");
	loadSoundBuffer("sounds/reloadPistol.ogg");
	loadSoundBuffer("sounds/pistolShot.ogg");
	loadSoundBuffer("sounds/reloadShotgun.ogg");
	loadSoundBuffer("sounds/shotgunShot.ogg");
}

void shutdown()
{
	sounds.clear();
}

void playAmbientSound(const std::string& filePath)
{
	removeStoppedSounds();
	if(sceneMute)
		return;

	SoundData soundData = soundDataHolder.getSoundData(filePath);
	playSound(filePath, soundVolume * soundData.mVolumeMultiplier, soundData.mLoop);
}

void playSpatialSound(const std::string& filePath, sf::Vector2f soundPosition)
{
	removeStoppedSounds();
	if(sceneMute)
		return;

	SoundData soundData = soundDataHolder.getSoundData(filePath);
	float spatialVolume = spatializationManager.getSpatialVolume(soundData, soundPosition, soundVolume);
	playSound(filePath, spatialVolume, soundData.mLoop);
}

void setSceneMute(bool mute)
{
	sceneMute = mute;
}

bool isMuted()
{
	return sceneMute;
}

void setListenerPosition(sf::Vector2f listenerPosition)
{
	spatializationManager.setListenerPosition(listenerPosition);
}

void setMuted(bool mute)
{
	// TODO: Get rid of this static var
	static float previousVolume = soundVolume;
	setVolume(mute ? 0.f : previousVolume);
	areSoundsMuted = mute;
}

void setVolume(float volume)
{
	soundVolume = volume;
	for(auto& sound : sounds)
		sound.setVolume(volume);
}

float getVolume()
{
	return soundVolume;
}

void removeEverySound()
{
	sounds.clear();
}

}

