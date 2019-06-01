#include "soundDataHolder.hpp"

using PopHead::Audio::SoundDataHolder;
using PopHead::Audio::SoundData;

SoundDataHolder::SoundDataHolder()
{
	mAllSoundsData["resources/sounds/barretaShot.wav"] = SoundData(1.4f);
}

SoundData SoundDataHolder::getSoundData(const std::string& filePath)
{
	auto found = mAllSoundsData.find(filePath);
	return found->second;
}