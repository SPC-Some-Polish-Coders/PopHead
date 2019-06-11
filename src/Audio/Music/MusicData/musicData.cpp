#include "musicData.hpp"

using ph::Audio::MusicData;

MusicData::MusicData(float volumeMultiplier, bool loop)
	: mVolumeMultiplier(volumeMultiplier)
	, mLoop(loop)
{
}
