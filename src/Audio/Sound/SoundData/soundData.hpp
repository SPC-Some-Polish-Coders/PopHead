#ifndef POPHEAD_AUDIO_SOUNDDATA_H_
#define POPHEAD_AUDIO_SOUNDDATA_H_

namespace PopHead {
namespace Audio {

struct SoundData
{
	float mVolumeMultiplier;
	float mMin;
	float mMax;
	bool mLoop;

	SoundData(const float volumeMultiplier = 1.f, const bool loop = false, const float min = 0.f, const float max = 1000.f);
};

}}

#endif
