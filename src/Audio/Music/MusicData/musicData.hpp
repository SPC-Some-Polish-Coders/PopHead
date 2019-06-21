#pragma once

#include <string>

namespace ph {

struct MusicData
{
	float mVolumeMultiplier;
	bool mLoop;

	MusicData(float volumeMultiplier = 1.f, bool loop = false);
};

}
