#include "soundData.hpp"

using PopHead::Audio::SpatialSoundData;
using PopHead::Audio::SoundData;

SoundData::SoundData(float volumeMultiplier, bool loop)
	:mVolumeMultiplier(volumeMultiplier)
	,mLoop(loop)
{
}

SpatialSoundData::SpatialSoundData(float volumeMultiplier, float min, float max, bool loop)
	:SoundData(volumeMultiplier, loop)
	,mMin(min)
	,mMax(max)
{
}