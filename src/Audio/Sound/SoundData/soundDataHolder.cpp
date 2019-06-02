#include "soundDataHolder.hpp"

using PopHead::Audio::SoundDataHolder;
using PopHead::Audio::SoundData;

SoundDataHolder::SoundDataHolder()
{
	mAllSoundsData["resources/sounds/barretaShot.wav"] = SoundData(1.4f);
	mAllSoundsData["resources/sounds/zombieGetsAttacked.wav"] = SoundData(5.f, false, 200.f, 500.f);
}

SoundData SoundDataHolder::getSoundData(const std::string& filePath)
{
	auto found = mAllSoundsData.find(filePath);
	return found->second;
}