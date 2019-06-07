#include "soundData.hpp"

using PopHead::Audio::SoundData;

SoundData::SoundData(const float volumeMultiplier, const bool loop, const float min, const float max)
	:mVolumeMultiplier(volumeMultiplier)
	,mLoop(loop)
	,mMin(min)
	,mMax(max)
{
}
