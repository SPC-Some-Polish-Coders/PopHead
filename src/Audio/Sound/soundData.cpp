#include "soundData.hpp"

using PopHead::Audio::SpatialSoundData;
using PopHead::Audio::SoundData;

SoundData PopHead::Audio::getAmbientSoundData(const std::string& filePath)
{
	if(filePath == "resources/sounds/barretaShot.wav")
		return SoundData(1.4f);
	else
		return SoundData();
}

SpatialSoundData PopHead::Audio::getSpatialSoundData(const std::string& filePath)
{
	return SpatialSoundData();
}

SoundData::SoundData(float volumeMultiplier, bool loop)
	:mVolumeMultiplier(volumeMultiplier)
	,mLoop(loop)
{
}

SpatialSoundData::SpatialSoundData(float volumeMultiplier, bool loop, float min, float max)
	:SoundData(volumeMultiplier, loop)
	,mMin(min)
	,mMax(max)
{
}
