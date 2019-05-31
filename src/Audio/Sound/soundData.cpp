#include "soundData.hpp"

using PopHead::Audio::SoundData;

SoundData::SoundData(float volumeMultiplier, float min, float max, bool loop)
	:mVolumeMultiplier(volumeMultiplier)
	,mMin(min)
	,mMax(max)
	,mLoop(loop)
{
}
