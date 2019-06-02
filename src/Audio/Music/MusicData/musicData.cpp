#include "musicData.hpp"

using PopHead::Audio::MusicData;

MusicData::MusicData(float volumeMultiplier, bool loop)
	: mVolumeMultiplier(volumeMultiplier)
	, mLoop(loop)
{
}
