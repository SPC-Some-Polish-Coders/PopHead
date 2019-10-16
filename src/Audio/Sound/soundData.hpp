#pragma once

#include <map>
#include <string>

namespace ph {

struct SoundData
{
	float mVolumeMultiplier;
	float mMaximalFullVolumeDistance;
	float mMaximalHearableDistance;	
	bool mLoop;

	SoundData(const float volumeMultiplier = 1.f, const bool loop = false,
			const float maximalFullVolumeDistance = 0.f, const float maximalHearableDistance = 1000.f);
};

class SoundDataHolder
{
public:
	SoundDataHolder();

	auto getSoundData(const std::string& filePath)->SoundData;

private:
	std::map<std::string, SoundData> mAllSoundsData;
};

}
