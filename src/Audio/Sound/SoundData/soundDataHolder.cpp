#include "soundDataHolder.hpp"

using PopHead::Audio::SoundDataHolder;
using PopHead::Audio::SoundData;

SoundDataHolder::SoundDataHolder()
{
	mAllSoundsData["sounds/barretaShot.wav"] = SoundData(1.4f);
	mAllSoundsData["sounds/zombieGetsAttacked.wav"] = SoundData(5.f, false, 200.f, 1000.f);
}

SoundData SoundDataHolder::getSoundData(const std::string& filePath)
{
	auto found = mAllSoundsData.find(filePath);
	return found->second;
}