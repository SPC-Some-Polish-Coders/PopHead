#include "soundData.hpp"

using PopHead::Audio::SoundData;

SoundData PopHead::Audio::getSoundData(const std::string& filePath)
{
	if(filePath == "resources/sounds/barretaShot.wav")
		return SoundData(1.4f);
	else
		return SoundData();
}

SoundData::SoundData(float volumeMultiplier, bool loop, float min, float max)
	:mVolumeMultiplier(volumeMultiplier)
	,mLoop(loop)
	,mMin(min)
	,mMax(max)
{
}
