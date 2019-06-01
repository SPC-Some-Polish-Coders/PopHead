#ifndef POPHEAD_AUDIO_SOUNDDATA_H_
#define POPHEAD_AUDIO_SOUNDDATA_H_

#include <string>

namespace PopHead {
namespace Audio {

struct SoundData
{
	float mVolumeMultiplier;
	bool mLoop;
	float mMin;
	float mMax;

	SoundData(float volumeMultiplier = 1.f, bool loop = false, float min = 0.f, float max = 1000.f);
};

SoundData getSoundData(const std::string& filePath);

}}

#endif
