#include "soundPlayer.hpp"
#include "Logs/logs.hpp"
#include "Resources/resourceHolder.hpp"
#include "Audio/Sound/soundData.hpp"
#include "Audio/Sound/spatializationManager.hpp"
#include <SFML/Audio.hpp>
#include <list>

namespace ph {

namespace SoundPlayer
{

namespace {
	std::list<sf::Sound> sounds;
	SoundBufferHolder soundBuffers;
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
	sound.setBuffer(soundBuffers.get(filePath));
	sound.setVolume(volume);
	sound.setLoop(loop);
	sounds.emplace_back(std::move(sound));
	sounds.back().play();
}

void init()
{
	setMuted(false);

	soundBuffers.load("sounds/swordAttack.wav");
	soundBuffers.load("sounds/carTireScreech.ogg");
	soundBuffers.load("sounds/zombieGrowl1.ogg");
	soundBuffers.load("sounds/zombieGrowl2.ogg");
	soundBuffers.load("sounds/zombieGrowl3.ogg");
	soundBuffers.load("sounds/zombieGrowl4.ogg");
	soundBuffers.load("sounds/reloadPistol.ogg");
	soundBuffers.load("sounds/pistolShot.ogg");
	soundBuffers.load("sounds/reloadShotgun.ogg");
	soundBuffers.load("sounds/shotgunShot.ogg");
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
	return isMuted;
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

}

