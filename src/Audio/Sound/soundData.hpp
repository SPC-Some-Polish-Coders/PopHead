#ifndef POPHEAD_AUDIO_SOUNDDATA_H_
#define POPHEAD_AUDIO_SOUNDDATA_H_

#include <string>

namespace PopHead {
namespace Audio {

struct SoundData
{
	float mVolumeMultiplier;
	bool mLoop;

	SoundData(float volumeMultiplier = 1.f, bool loop = false);
};

struct SpatialSoundData : public SoundData
{
	float mMin;
	float mMax;

	SpatialSoundData(float volumeMultiplier = 1.f, bool loop = false, float min = 0.f, float max = 1000.f);
};

SoundData getAmbientSoundData(const std::string& filePath);
SpatialSoundData getSpatialSoundData(const std::string& filePath);

}}

#endif
