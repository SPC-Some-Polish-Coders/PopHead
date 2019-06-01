#include "soundData.hpp"

using PopHead::Audio::SoundData;

SoundData::SoundData(float volumeMultiplier, bool loop, float min, float max)
	:mVolumeMultiplier(volumeMultiplier)
	,mLoop(loop)
	,mMin(min)
	,mMax(max)
{
}
