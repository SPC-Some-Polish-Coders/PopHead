#include "soundData.hpp"

ph::SoundData::SoundData(const float volumeMultiplier, const bool loop,
					const float maximalFullVolumeDistance, const float maximalHearableDistance)
	:mVolumeMultiplier(volumeMultiplier)
	,mLoop(loop)
	,mMaximalFullVolumeDistance(maximalFullVolumeDistance)
	,mMaximalHearableDistance(maximalHearableDistance)
{
}
