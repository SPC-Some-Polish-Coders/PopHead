#ifndef POPHEAD_AUDIO_SOUNDDATA_H_
#define POPHEAD_AUDIO_SOUNDDATA_H_

namespace PopHead {
namespace Audio {

struct SoundData
{
	float mVolumeMultiplier;
	float mMaximalFullVolumeDistance;
	float mMaximalHearableDistance;
	bool mLoop;

	SoundData(const float volumeMultiplier = 1.f, const bool loop = false,
			const float maximalFullVolumeDistance = 0.f, const float maximalHearableDistance = 1000.f);
};

}}

#endif
