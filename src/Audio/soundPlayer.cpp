#include "pch.hpp"
#include "soundPlayer.hpp"
#include "Resources/soundBufferHolder.hpp"

namespace ph::SoundPlayer
{

struct SoundData
{
	float volumeMultiplier;
	float maxFullVolumeDistance;
	float maxHearableDistance;	
	bool loop;
};

namespace {
	std::list<sf::Sound> sounds;
	std::map<std::string, SoundData> soundsData;
	Vec2 listenerPos;
	float soundVolume = 15.f;
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
	loadSoundBuffer("sounds/swordAttack.wav");
	loadSoundBuffer("sounds/zombieGrowl1.ogg");
	loadSoundBuffer("sounds/zombieGrowl2.ogg");
	loadSoundBuffer("sounds/zombieGrowl3.ogg");
	loadSoundBuffer("sounds/zombieGrowl4.ogg");
	loadSoundBuffer("sounds/reloadPistol.ogg");
	loadSoundBuffer("sounds/pistolShot.ogg");
	loadSoundBuffer("sounds/reloadShotgun.ogg");
	loadSoundBuffer("sounds/shotgunShot.ogg");

	// init sound data
	Xml soundDataXml;
	PH_ASSERT_CRITICAL(soundDataXml.loadFromFile("resources/sounds/soundData.xml"), "\"resources/sounds/soundData.xml\" wasn't loaded correctly!");
	const auto soundDataNode = soundDataXml.getChild("soundData");
	const std::vector<Xml> soundNodes = soundDataNode->getChildren("sound");
	for(auto& soundNode : soundNodes) 
	{
		std::string fileName = soundNode.getAttribute("fileName")->toString();
		std::string filePath = "sounds/" + fileName;
		SoundData data;
		data.volumeMultiplier = soundNode.getAttribute("volumeMultiplier")->toFloat();
		data.loop = soundNode.getAttribute("loop")->toBool();
		data.maxFullVolumeDistance = soundNode.getAttribute("maxFullVolumeDistance")->toFloat();
		data.maxHearableDistance = soundNode.getAttribute("maxHearableDistance")->toFloat();
		soundsData[filePath] = data;
	}
}

void shutdown()
{
	sounds.clear();
}

static void removeStoppedSounds()
{
	sounds.remove_if([](const sf::Sound& sound) 
	{
		return sound.getStatus() == sf::Sound::Status::Stopped;
	});
}

void playAmbientSound(const std::string& filePath)
{
	removeStoppedSounds();
	SoundData soundData = soundsData[filePath];
	playSound(filePath, soundVolume * soundData.volumeMultiplier, soundData.loop);
}

void playSpatialSound(const std::string& filePath, Vec2 soundPos)
{
	removeStoppedSounds();

	SoundData soundData = soundsData[filePath];

	float maxVolume = soundVolume * soundData.volumeMultiplier;
	float distanceBetweenListenerAndSoundSource = distanceBetweenPoints(listenerPos, soundPos);
	
	float spatialVolume;
	if(distanceBetweenListenerAndSoundSource > soundData.maxHearableDistance)
	{
		spatialVolume =  0.f;
	}
	else if(distanceBetweenListenerAndSoundSource < soundData.maxFullVolumeDistance)
	{
		spatialVolume =  maxVolume;
	}
	else
	{
		float scope = soundData.maxHearableDistance - soundData.maxFullVolumeDistance;
		float distanceBetweenListenerAndMaxVolumeArea = distanceBetweenListenerAndSoundSource - soundData.maxFullVolumeDistance;
		float distanceBetweenListenerAndHearableAreaBound = scope - distanceBetweenListenerAndMaxVolumeArea;
		float spatializationFactor = distanceBetweenListenerAndHearableAreaBound / scope;
		spatialVolume =  maxVolume * spatializationFactor;
	}

	playSound(filePath, spatialVolume, soundData.loop);
}

void setListenerPosition(Vec2 pos)
{
	listenerPos = pos;
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

}

