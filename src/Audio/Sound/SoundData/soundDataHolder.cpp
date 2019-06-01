#include "soundDataHolder.hpp"

using PopHead::Audio::SoundDataHolder;
using PopHead::Audio::SoundData;

SoundData SoundDataHolder::getSoundData(const std::string& filePath)
{
	if(filePath == "resources/sounds/barretaShot.wav")
		return SoundData(1.4f);
	else
		return SoundData();
}